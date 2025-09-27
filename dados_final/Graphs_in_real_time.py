''' This code was used to plot the real time graph from the data
 we were receiving to detect errrors, bugs or interferences
'''


import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd

#Path of the file used to store the data
ficheiro_txt = r"C:\Users\inesm\Desktop\LimaSat25\dados_final\Final - Raw data from.txt"

# Creates the graph
fig, ax = plt.subplots()
x_data, y_data = [], []  # Lists to store the data

def ler_dados():

    """This functions reads the file and returns lists of x and y"""
    try:
        df = pd.read_csv(ficheiro_txt, header= None, names=["Temperature", "Pressure","Altitude","UV1","UV2","UV3","UV4","SumUV","Time", "Sum", "checksum", "Verification"])
        return df["Time"].tolist(), df["UV4"].tolist()   # change the names to create graphs with other values
    except FileNotFoundError:
        print(f"Error: file {ficheiro_txt} not found.")
        return [], []
    except pd.errors.EmptyDataError:
        return [], []
    
def atualizar(frame):
    """Refreshes the graph"""
    global x_data, y_data
    x_data, y_data = ler_dados()

    ax.clear()
    ax.plot(x_data, y_data, marker="o", linestyle="-", color="b")
    ax.set_title("Graph in real time")
    ax.set_xlabel("Time")
    ax.set_ylabel("UV4 value")
    ax.grid(True)

#Creates the animation that refreshes the graph

ani = animation.FuncAnimation(fig, atualizar, interval=1000)

plt.show()