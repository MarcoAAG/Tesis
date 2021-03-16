from scipy import signal
import matplotlib.pyplot as plt
import control
import math
import numpy as np


def transferFunction(num, den):
    G = control.tf(num,den)
    Gc = control.feedback(G)
    return G

if __name__ == "__main__":
    
    #!PLanta del sistema
    numG = [-1]
    denG = [0.01484, 0, -4.155]
    G = transferFunction(numG, denG)

    #!PID controller de la forma
    #! Gc = (kds^2 + Kps + Ki) / s
    #! Kp = -44.6008
    #! Ki = -67.33
    #! Kd = -15.94
    numC = [-15.94, -44.6008, -67.33]
    denC = [1, 0]
    Gc = transferFunction(numC, denC)
    
    #!Crear lazo cerrado
    Gs = control.series(G,Gc) 
    Gh = control.feedback(Gs)

    T, y = control.step_response(Gh)
    plt.plot(T, y, linewidth = 2.0)

    plt.xlabel('Tiempo [s]')
    plt.ylabel('Amplitud')
    plt.title('Respuesta escalon')
    plt.grid()
    plt.legend()
    plt.show()