fetch->decode->execute->checkInterrupt
|----------------------|
no se puede interrumpir
Process Control Block --> Tiene una foto de la CPU en el momento en el que se quita el proceso de running
la foto contiene : PID, ProgramCounter, Quantum, Contenido de los registros



Registro CPU 32 bits

EAX -> 10100011 10101110 10100011 10101110 -> 32bits => ocupa 4 posiciones de memoria

Puede que los cuatro bytes de EAX no esten almacenados en la misma página
por ejemplo:

página 1:
0xfffe 10100011
0xffff 10101110 

página 2:
0x0001 10100011
0x0002 10101110



Fetch:
La primera etapa del ciclo consiste en buscar la próxima instrucción a ejecutar. En este trabajo
práctico cada instrucción deberá ser pedida al módulo Memoria* utilizando el Program Counter
(también llamado Instruction Pointer) que representa el número de instrucción a buscar relativo al
proceso en ejecución (sería la línea del archivo de pseudocódigo). Al finalizar el ciclo, 
este último deberá ser actualizado (sumarle 1) si corresponde.

*tener en cuenta que memoria ya tiene una lista de intrucciones correspondientes al proceso a ejecutar
y únicamente cpu le intica a memoria que le pase la próxima instruccion. Decirle a memoria qué conjunto
pasarle a COU es responsabilidad del kernel


-Problema: las instrucciones toman distintos tipos y cantidades de argumentos

SET           AX 1
RESIZE        8 
IO_STDIN_READ Interfaz Registro Tamanio

-Solución Posible 1:

Recibimos de memoria:
Operacion               --> "SET"
Struct con los argumentos --> struct.Registro = "AX"  struct.valor = "1"

Las instrucciones toman distinta cantidad y tipos de argumentos
el switch del execute() se va a encargar de trabajar con la instruccion y el struct correspondiente
que tendríamos que recibir de memoria con los argumentos

-Solucion Posible 2: Esta fue elegida

Recibimos de memoria:
Operacion   --> arr[0] = "SET"
Argumentos  --> arr[1] = "AX", arr[2] = "1", arr[3] = vacio, arr[4] = vacio, arr[5] = vacio
Instruccion --> arr[6] = {"SET","AX","1","","",""}

La idea sería recibir un array de strings con lo que se mande memoria donde el primer elemento del array
es la operacion en sí y el resto los operandos. Jamás vas a acceder a un elemento del array que no necesite
la operación que recibiste porque el switch en el execute() sólo va a pedir los elementos requeridos como argumentos
de la operacion. Esto me siplifica la vida para codearlo, aprovechando que todo viene como string desde memoria
no me pareció necesario un struct, ya que la estructura es útil cuando querés almacenar datos de distinto tipo juntos. 



DUDAS:
-¿Vale sumar a un registro de 32 bits uno de 8? Introduciria complejidad a la funcion sum()