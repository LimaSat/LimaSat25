import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd

#Nome do ficheiro de dados a ser utilizado
ficheiro_txt = "dados"


# Criar a figura do gráfico
fig, ax = plt.subplots()
x_data, y_data = [], []  # Listas para armazenar os dados

def ler_dados():

    """Esta função vai ler o ficheiro e retorna listas separadas de x e y"""
    try:
        df = pd.read_csv(ficheiro_txt, header= None, names=["x", "y"])
        return df["x"].tolist(), df["y"].tolist()
    except FileNotFoundError:
        print(f"Erro: o ficheiro {ficheiro_txt} não foi encontrado.")
        return [], []
    except pd.errors.EmptyDataError:
        return [], []
    
def atualizar(frame):
    """Atualiza os dados e redesenha o gráfico"""
    global x_data, y_data
    x_data, y_data = ler_dados()

    ax.clear()
    ax.plot(x_data, y_data, marker="o", linestyle="-", color="b")
    ax.set_title("Gráfico Atualizado em Tempo Real")
    ax.set_xlabel("Tempo")
    ax.set_ylabel("Valor")
    ax.grid(True)

#Criar a animação que atualiza o gráafico a cada segundo

ani = animation.FuncAnimation(fig, atualizar, interval=1000)

plt.show()