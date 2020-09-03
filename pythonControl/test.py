from scipy import signal
import matplotlib.pyplot as plt

lti = signal.lti([1.0], [1.0, 0.0, 1.0])
t, y = signal.step(lti)
plt.plot(t, y)
plt.xlabel('Tiempo [s]')
plt.ylabel('Amplitud')
plt.title('Respuesta escalon de Pitch')
plt.grid()
plt.show()
