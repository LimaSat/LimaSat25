import serial
import csv
import os
import time


# IMPORTANT: Set the correct port where your Arduino is connected
port = 'COM6'  # Update with your Arduino's port, it's easy to check on the arduino IDE
baudrate = 9600

# Check if the Arduino is connected
while True:
    try:
        ser = serial.Serial(port, baudrate)
        break
    except serial.SerialException:
        print(f"Could not open port {port}. Please check if the Arduino is connected, the port is correct, and try reconnecting the Arduino.")
    time.sleep(3)




folder_path = r"C:\Users\Aluno\Documents\cansat" # IMPORTANT: change this to the folder path where you want the file to go

os.makedirs(folder_path, exist_ok=True)  # I think it's suposed to check if the folder exists and create it if it doesn't

file_name = "sensor_data.csv" # Change this if you want a diferent name for the file created or a diferent type for exemple .txt

file_path = os.path.join(folder_path, file_name)


#WARNING: if there is already a "file_name" file this will overwrite it,
# if you got useful data don't forget to rename it before using this program again 



# Open a csv file to save the data
with open(file_path, 'w', newline='') as file:
    writer = csv.writer(file)
    
    # Write header in csv file
    writer.writerow(['Temperature', 'Pressure', 'Altitude', 'Humidity', 'UV1', 'UV2', 'UV3', 'UV4', 'MaxUV', 'Time'])
    print('Temperature', 'Pressure', 'Altitude', 'Humidity', 'UV1', 'UV2', 'UV3', 'UV4', 'MaxUV', 'Time')

    print("Reading from Arduino... Press Ctrl+C to stop.")
    
    while True:
        # Read data from Arduino (each line will be terminated by a newline)
        try:
            line = ser.readline().decode('utf-8').strip()
            print(line)  # Print the data to the console
            
            # Write the data to the file
            writer.writerow(line.split(','))

        except KeyboardInterrupt:
            print("Program interrupted. Exiting...")
            break

ser.close()  # Close the serial connection
