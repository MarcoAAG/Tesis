from scipy import signal
import matplotlib.pyplot as plt
import control
import math
import numpy as np




def PIController(Ts, Mp):
    #! Planta
    numG = [10]
    denG = [1, 0]
    G = control.tf(numG,denG)
    
    #!Parametro de diseo para control PI
    # Ts = 1.3    #* Tiempo de asentamiento
    # Mp = 2.0      #* Porcentaje de sobre impulso
    # Tol = 5     #* Tolerancia de error %
    z = math.sqrt((np.log(Mp/100.0))**2 / ((math.pi ** 2) + (np.log(Mp/100.0))**2))     #* Calcular Factor de amortiguamiento
    Wn = 5.0 / (z*Ts)       #*Calcular frecuencia natural
    
    #! Calcular Polos Deseados
    numD = [Wn**2]
    denD = [1.0, 2.0 * z * Wn, Wn**2]
    Gd = control.tf(numD, denD)
    
    #! Calcular Ganancias
    # Kc = ((2.0 * z * Wn) - 1.0)/10.0
    # Ti = Kc/(Wn**2) * 10.0
    Kc = (2.0 * z * Wn) / 10.0
    Ti = 10.0*Kc / (Wn**2)
    print("--------------------------------------------------")
    print("Ts = ", Ts)
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

    TS = [0.1,0.5,1.0,1.5,2.0,3.0]
    # MP = [0.1, 0.1]
    for i in range(0,len(TS)):
        C = PIController(TS[i], 0.10)
        T, y = control.step_response(C)
        plt.plot(T, y)

    # C1 = PIController(1.3, 0.1)
    # T, y = control.step_response(C1)
    # plt.plot(T, y)

    # C2 = PIController(2.0, 2.0)
    # T, y = control.step_response(C2)
    # plt.plot(T, y)

    plt.xlabel('Tiempo [s]')
    plt.ylabel('Amplitud')
    plt.title('Respuesta escalon de Pitch')
    plt.grid()
    plt.show()





# lti = signal.lti([14.8], [1.0, 6, 14.8])
# print(control.dcgain(lti))
# t, y = signal.step(lti)
# plt.plot(t, y)
# plt.xlabel('Tiempo [s]')
# plt.ylabel('Amplitud')
# plt.title('Respuesta escalon de Pitch')
# plt.grid()
# plt.show()
