"""Capture ESP32 STATUS samples, save CSV, and validate readback."""

import math
import serial
import time
import csv
from pathlib import Path


#Serial connection settings
PORT = "COM3"
BAUD_RATE = 115200
READ_TIMEOUT_S = 1.0
WRITE_TIMEOUT_S = 1.0
RESPONSE_TIMEOUT_S = 3.0
MAX_LINE_BYTES = 128
STARTUP_DELAY_S = 2.0
#Sample settings and the fields saved in each CSV row
SAMPLE_COUNT = 5
POLL_INTERVAL_S = 1.0
CSV_FIELDS = ["sample", "elapsed_s", "position", "homed", "mode"]
#Keeps the logs beside this script regardless of where I run it
LOG_DIR = Path(__file__).resolve().parent / "logs"


#Opens the port without requesting a reset through DTR or RTS
def connect_to_board():
    connection = serial.Serial()
    connection.port = PORT
    connection.baudrate = BAUD_RATE
    connection.timeout = READ_TIMEOUT_S
    connection.dtr = False
    connection.rts = False
    connection.write_timeout = WRITE_TIMEOUT_S
    connection.open()
    return connection
#Adds the newline our firmware needs before handling a command
def send_command(connection, command):
    command+= "\n"
    payload = command.encode("utf-8")
    return connection.write(payload)

#Waits for a complete STATUS line and skips other serial messages
def read_status(connection):
    deadLine = time.monotonic() + RESPONSE_TIMEOUT_S
    while time.monotonic() < deadLine:
        raw_line = connection.read_until(b"\n", size=MAX_LINE_BYTES)
        if not raw_line.endswith(b"\n"):
            continue
        line = raw_line.decode("utf-8", errors="replace").strip()
        if line.startswith("STATUS position="):
            return line
    raise TimeoutError("No status response received.")


#Checks the STATUS fields and converts the position and home flag
def parse_status_line(line):
    parts = line.split()
    if len(parts) != 4 or parts[0] != "STATUS":
        raise ValueError("Invalid status format.")
    fields = {}
    for part in parts:
        if "=" in part:
            key, value = part.split("=", 1)
            fields[key] = value
    if set(fields) != {"position", "homed", "mode"}:
        raise ValueError("Invalid STATUS fields")
    position = int(fields["position"])
    fields["position"] = position
    if fields["homed"] not in {"0", "1"}:
        raise ValueError("Invalid homed value")
    homed = fields["homed"] == "1"
    fields["homed"] = homed
    if fields["mode"] not in {"IDLE", "MOVING", "HOMING"}:
        raise ValueError("Invalid mode value")
    return fields

#Requests one status report and returns its parsed values
def query_status(connection):
    send_command(connection, "STATUS")
    status_line = read_status(connection)
    status = parse_status_line(status_line)
    return status

#Checks the saved header and rows, then restores their value types
def load_records_csv(input_path):
    with open(input_path, "r", newline="", encoding="utf-8") as csv_file:
        reader = csv.DictReader(csv_file)
        if reader.fieldnames != CSV_FIELDS:
            raise ValueError("CSV fieldnames do not match expected fields")
        records = [row for row in reader]
        for record in records:
            if None in record or None in record.values():
                raise ValueError("Missing values in CSV")
            record["sample"] = int(record["sample"])
            record["elapsed_s"] = float(record["elapsed_s"])
            record["position"] = int(record["position"])
            if record["homed"] not in {"True", "False"}:
                raise ValueError("Invalid homed value in CSV")
            record["homed"] = record["homed"] == "True"
            if record["mode"] not in {"IDLE", "MOVING", "HOMING"}:
                raise ValueError("Invalid mode value in CSV")
            
        return records


#Checks sample count, fields, numbering and increasing elapsed times
def validate_records(records, expected_count):
    if len(records) != expected_count:
        raise ValueError(f"Expected {expected_count} records, got {len(records)}")
    previous_elapsed_s = None
    for expected_sample, record in enumerate(records, start=1):
        if not isinstance(record, dict):
            raise ValueError("Record is not a dictionary")
        if set(record.keys()) != set(CSV_FIELDS):
            raise ValueError("Record fields do not match expected fields")
        if not isinstance(record["sample"], int):
            raise ValueError("Sample field is not an integer")
        if record["sample"] != expected_sample:
                    raise ValueError(f"Expected sample {expected_sample}, got {record['sample']}")
        if not isinstance(record["elapsed_s"], float):
            raise ValueError("Elapsed_s field is not a float")
        elapsed_s = record["elapsed_s"]
        if not math.isfinite(elapsed_s) or elapsed_s < 0:
            raise ValueError("Elapsed_s field is not a non-negative finite float")
        if previous_elapsed_s is not None and elapsed_s <= previous_elapsed_s:
            raise ValueError("Elapsed times must increase")
        previous_elapsed_s = elapsed_s
        if not isinstance(record["position"], int):
            raise ValueError("Position field is not an integer")
        if not isinstance(record["homed"], bool):
            raise ValueError("Homed field is not a boolean")
        if record["mode"] not in {"IDLE", "MOVING", "HOMING"}:
            raise ValueError("Invalid mode value in record")

#Writes the header and sample records in our selected field order
def save_records_csv(records, output_path):
    with open(output_path, "w", newline="", encoding="utf-8") as csv_file:
         writer = csv.DictWriter(csv_file, fieldnames=CSV_FIELDS)
         writer.writeheader()
         writer.writerows(records)

#Collects the samples, closes the port, then saves and checks the CSV
def main():
    connection = connect_to_board()
    try:
        print(connection.port)
        print(connection.is_open)
        time.sleep(STARTUP_DELAY_S)
        connection.reset_input_buffer()
        #Uses elapsed time that is not affected by clock adjustments
        start_time = time.monotonic()
        records = []
        for sample in range(SAMPLE_COUNT):
            status = query_status(connection)
            elapsed_s = time.monotonic() - start_time
            record = {
    "sample": sample + 1,
    "elapsed_s": elapsed_s,
    "position": status["position"],
    "homed": status["homed"],
    "mode": status["mode"],
}
            records.append(record)
            print (sample + 1, status, elapsed_s)
            time.sleep(POLL_INTERVAL_S)
        print(len(records))

    #Closes the serial port even if collecting a sample fails
    finally:
        connection.close()
    print(connection.is_open)
    LOG_DIR.mkdir(parents=True, exist_ok=True)
    output_path = LOG_DIR / f"status_{time.time_ns()}.csv"
    save_records_csv(records, output_path)
    print(output_path)
    loaded_records = load_records_csv(output_path)
    validate_records(loaded_records, SAMPLE_COUNT)
    print(f"Loaded {len(loaded_records)} records")

#Runs the client directly and reports expected errors with a failure exit code
if __name__ == "__main__":
    try:
        main()
    except (serial.SerialException, TimeoutError, ValueError, OSError) as error:
        print(f"Error: {error}")
        raise SystemExit(1)