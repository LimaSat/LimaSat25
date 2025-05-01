import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd

#Nome do ficheiro de dados a ser utilizado
ficheiro_txt = r"C:\Users\Utilizador\Desktop\dados2.txt"


# Criar a figura do gráfico
fig, ax = plt.subplots()
x_data, y_data = [], []  # Listas para armazenar os dados

def ler_dados():

    """Esta função vai ler o ficheiro e retorna listas separadas de x e y"""
    try:
        df = pd.read_csv(ficheiro_txt, header= None, names=["Temperatura", "Pressão","Altitude","UV1","UV2","UV3","UV4","MaxUV","Tempo", "soma", "checksum", "verificação"])
        return df["Tempo"].tolist(), df["UV4"].tolist()
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
    ax.set_ylabel("Temperatura")
    ax.grid(True)

#Criar a animação que atualiza o gráafico a cada segundo

ani = animation.FuncAnimation(fig, atualizar, interval=1000)

plt.show()