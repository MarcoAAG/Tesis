from scipy import signal
import matplotlib.pyplot as plt
import control
import math
import numpy as np

def planta(num, den):
    G = control.tf(num,den)
    Gc = control.feedback(G)
    return G

def PIController(Ts, Mp, G):
    # #! Planta
    # numG = [10]
    # denG = [1, 0]
    # G = control.tf(numG,denG)
    
    #!Parametro de diseo para control PI
    # Ts = 1.3    #* Tiempo de asentamiento
    # Mp = 2.0      #* Porcentaje de sobre impulso
    # Tol = 5     #* Tolerancia de error %
    z = math.sqrt((np.log(Mp/100.0))**2 / ((math.pi ** 2) + (np.log(Mp/100.0))**2))     #* Calcular Factor de amortiguamiento
    Wn = 6.0 / (z*Ts)       #*Calcular frecuencia natural
    
    #! Calcular Polos Deseados
    numD = [Wn**2]
    denD = [1.0, 2.0 * z * Wn, Wn**2]
    Gd = control.tf(numD, denD)
    
    #! Calcular Ganancias
    Kc = ((2.0 * z * Wn) - 1.0)/numG[0]
    Ti = Kc/(Wn**2) * numG[0]
    # Kc = (2.0 * z * Wn) / numG[0]
    # Ti = numG[0]*Kc / (Wn**2)
    print("--------------------------------------------------")
    print("Ts = ", Ts)
    print(Gd)
    print(control.pole(Gd))
    print("Ganancia Kp = ", Kc) 
    print("Ganancia Ki = ", Ti)

    #! Obtener Funcion de transferencia de control PI
    numC = [Kc*Ti, Kc]
    denC = [Ti, 0.0]
    Gc = control.tf(numC, denC)
    
    #!Crear lazo cerrado
    Gs = control.series(G,Gc) 
    Gh = control.feedback(Gs)
    
    return(Gh)

if __name__ == "__main__":

    numG = [125]
    denG = [1, 0]
    G = planta(numG,denG)

    # T, y = control.step_response(G)
    # plt.plot(T, y, linewidth=2.0)

    # TS = [0.2,0.5,1.0,1.5,2.0,3.0]
    # for i in range(0,len(TS)):
        # C = PIController(TS[i], 0.10, G)
        # T, y = control.step_response(C)
        # tmp = 'Ts = ' + str(TS[i])
        # plt.plot(T, y,label = tmp, linewidth=2.0)

    C = PIController(0.3,0.1,G)
    T, y = control.step_response(C)
    plt.plot(T, y, linewidth = 2.0)

    # plt.xlabel('Tiempo [s]')
    # plt.ylabel('Amplitud')
    # plt.title('Respuesta escalon de Pitch')
    plt.grid()
    # plt.legend()
    plt.show()
