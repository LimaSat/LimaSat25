import serial
import csv
import time

# Set the correct port where your Arduino is connected
port = 'COM3'  # Update with your Arduino's port
baudrate = 9600
ser = serial.Serial(port, baudrate)

# Open a CSV or TXT file to save the data
file_name = "sensor_data.csv"  # You can change this to .txt if you want
with open(file_name, 'w', newline='') as file:
    writer = csv.writer(file)
    
    # Write header if CSV
    writer.writerow(['Temperature', 'Pressure', 'Altitude', 'Humidity', 'UV', 'Time'])

    print("Reading from Arduino... Press Ctrl+C to stop.")
    
    while True:
        # Read data from Arduino (each line will be terminated by a newline)
        try:
            line = ser.readline().decode('utf-8').strip()
            print(line)  # Print the data to the console
            
            # Check if the data is in CSV format or normal format
            if line.startswith("Temperature") or line.startswith("UV Sensor"):
                continue
            else:
                # Write the CSV data to file
                writer.writerow(line.split(','))

        except KeyboardInterrupt:
            print("Program interrupted. Exiting...")
            break

ser.close()  # Close the serial connection
