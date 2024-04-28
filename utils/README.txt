28/4 fran
Estructura para usar los hilos con conexiones del lado del servidor:
1-Abrir un socket
2-Pegarlo al puerto de escucha que quieras con bind()
3-Ponerlo para que escuche con listen()

Hasta acá está listo el el socket para dejarte recibir datos por el puerto escogido

3.1- en el medio está el protocolo del hanshake que es un mensaje simple
3.2-si querés muchos clientes lo que sigue va dentro de un while()
4-Esperás y aceptás a un cliente con accept()
5-Te creas un hilo para ese cliente con pthread_create()
    Le pasás como parámetro void* atender_cliente
    que se encarga de hacer lo que necesites con el cliente
    (le estás delegando a esa función el procesamiento del cliente)
6-ptread_detatch() para BUSCAAR PARA QUË SIRVE????

atender_cliente() : en esta función van los switch para ver qué 
toca hacer con la info transmitida por la conexion que te llegó,
las funciones para recibir datos se usan en ésta parte del programa

IMPORTANTE: al releer el enunciado me di cuenta de que las interfaces de IO no son clientes normales,
la consigna nos dice que el KERNEL le da instrucciones a las interfaces!!!
esto quiere decir que a pesar de que el kernel sea servidor de las interfaces, son las interfaces que están
a disposición de lo que quiera el kernel. La relación server cliente en éste caso no es como la de youtube con nosotros
porque nosotros le pedimos cosas a youtube y nos devuelve lo pedido, youtube no nos da órdenes a nosotros

Reflexiones: me estoy dando cuenta que necesitamos definir bien cómo sería un ciclo en nuestro programa
me está costando entender por dónde arrancaríamos a enviar información para que procesen los módulos 
A mi entender deberíamos arrancar por el kernel que tiene una "consola interactiva" a través de la cual
supongo que le diremos al kernel qué hacers