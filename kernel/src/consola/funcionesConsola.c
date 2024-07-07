#include <consola/funcionesConsola.h>


// ------------------------ FUNCIONES PRINCIPALES DE LA CONSOLA

void crear_proceso(char *path_proceso){
    
    int pid_asignado = _asignar_PID();
    t_pcb *nuevo_pcb = crear_pcb(pid_asignado, path_proceso);

    // ------------------ AGREGA A COLA NEW 
    pthread_mutex_lock(&new->mutex_cola);
        list_add(new->cola, nuevo_pcb);
    pthread_mutex_unlock(&new->mutex_cola);

    pthread_mutex_lock(&mutex_log);
        log_info(kernel_log, "Se crea el proceso < %d > en NEW", nuevo_pcb->pid);
    pthread_mutex_unlock(&mutex_log);

    sem_post(&proceso_cargado);
}

void extraer_proceso(int pid){
    // 1. Si el proceso esta en alguna cola de new o ready lo mando a exit instantaneamente, listo
    // 2. si esta en exec mando una interrumpcion a cpu y cuando vuelva lo mando a exit, listo
    // 3. si esta en la cola de bloqueados espero a que la IO me indique que termino de procesar la peticion de ese proceso para mandarlo a exit
    
    //t_estado *array_estados[] = {new, ready, exec, estado_exit}; -> DE EXIT NO LO PUEDO EXTRAER Y DE EJECUTANDO LO HAGO APARTE
    //pthread_mutex_t array_semaforos[] = {new->mutex_cola, ready->mutex_cola, exec->mutex_cola, estado_exit->mutex_cola};
    
    t_estado *array_estados[] = {new, ready};
    pthread_mutex_t array_semaforos[] = {new->mutex_cola, ready->mutex_cola};
    
    int cant_elementos = (int)(sizeof(array_estados) / sizeof(array_estados[0]));
    int resultado;

    if ((_esta_ejecutando(pid)))
    {
        enviar_interrupcion(INTERRUPCION); // LE MANDO A CPU LA INTERRUPCION DE FIN DE PROCESO
    }else
    {
        for (int i = 0; i < cant_elementos; i++)
        {   
            pthread_mutex_lock(&array_semaforos[i]);
                resultado = buscar_y_trasladar_pid(array_estados[i], pid);
            pthread_mutex_unlock(&array_semaforos[i]);
            
            if(resultado != (-1)){  break;  }
        }
    }

    // TODO: Agregar el caso de que lo busque en las colas de bloqueados y setee una variable global que tiene q chequear un proceso cuando vuelve de io para saber si puede seguir ejecutando. (punto 3 de mi lista)
}

void pausar_planificacion(void){ // Este mensaje se encargará de pausar la planificación de corto y largo plazo

    if (!planificacion_pausada)
    {
        planificacion_pausada = true;
    }else
    {
        puts("La planificacion ya esta pausada");
    }
    
}

void retomar_planificacion(void){
    if (planificacion_pausada)
    {   
        planificacion_pausada = false;

        for (int i = 0; i < contador_bloqueados; i++)
        {
            sem_post(&planificacion_en_pausa); // HAGO UN SIGNAL POR CADA HILO BLOQUEADO
        }
        contador_bloqueados = 0;
    }
}
// El tema de esto es que la cantidad de hilos en wait puede variar porque a veces puedo tener que justo un proceso vuelve de IO
// y el enunciado me dice: De la misma forma, los procesos bloqueados van a pausar su transición a la cola de Ready.
// Entonces pauso a los planificadores (2) y a veces a los q vuelven de IO(x).
//Entonces pauso toda transicion a ready y todo proceso devuelto por cpu.

void actualizar_grado_multiprog(int nuevo_valor){
    int valor_actual = config_kernel.GRADO_MULTIPROGRAMACION;
    int diferencia = nuevo_valor - valor_actual;

    if (diferencia > 0) // AUMENTO EL GRADO DE MULTIPROGRAMACION
    {
        for (int i = 0; i < diferencia; i++) // i CON VALOR POSITIVO
        {
            sem_post(&grado_multiprogramacion);
        }
        
    }else if (diferencia < 0) // REDUZCO EL GRADO DE MULTIPROGRAMACION
    {
        for (int i = diferencia; i > 0; i++) // i CON VALOR NEGATIVO
        {
            sem_wait(&grado_multiprogramacion);
        }
        
    }else
    {
        puts("El valor ingresado es igual al actual");
    }

    config_kernel.GRADO_MULTIPROGRAMACION = nuevo_valor;

    pthread_mutex_lock(&mutex_log);
        log_info(kernel_log, "Se actualizo grado de multiprogramacion a: < %d >", nuevo_valor);
    pthread_mutex_unlock(&mutex_log);
}

void _imprimir_estados_procesos(void){

    puts("Cola NEW: ");
    imprimir_cola(new);

    puts("Cola READY: ");
    imprimir_cola(ready);

    if (algoritmo_es_VRR()){
        puts("Cola READY +: ");
        imprimir_cola(ready_plus);
    }
    
    puts("Cola EXECUTE: ");
    imprimir_cola(exec);
    
    puts("Cola BLOCKED: ");
    imprimir_cola_bloqueados();

    puts("Cola EXIT: ");
    imprimir_cola(estado_exit);
}

void ejecutar_script(char *path){
    char* path_codigo = string_duplicate("/home/utnso/scripts-pruebas/");
    string_append(&path_codigo, path);

    char *leido = NULL; 
    size_t length = 0;    

    FILE* archivo = fopen(path_codigo, "r");
    pausar_planificacion();

    int caracteres_leidos = getline(&leido, &length, archivo);
    
    while (caracteres_leidos != (-1)){ 
        
        if (leido[caracteres_leidos - 1] == '\n') { leido[caracteres_leidos - 1] = '\0'; }
        char **array_del_comando = _interpretar(leido);
        t_comandos_consola opCode = _obtener_enum_comando(array_del_comando[0]);

        switch (opCode)
        {
            case INICIAR_PROCESO: // INICIAR_PROCESO [PATH]

                char * path_proceso = array_del_comando[1];
                crear_proceso(path_proceso);
                free(path_proceso);

                break;

            case FINALIZAR_PROCESO: // FINALIZAR_PROCESO [PID]
                int pid_proceso = atoi(array_del_comando[1]);
                extraer_proceso(pid_proceso);
                
                break;

            case DETENER_PLANIFICACION:
                pausar_planificacion();

                break;

            case INICIAR_PLANIFICACION:
                retomar_planificacion();

                break;

            case MULTIPROGRAMACION: // MULTIPROGRAMACION [VALOR]           
                int nuevo_valor_grado = atoi(array_del_comando[1]);
                actualizar_grado_multiprog(nuevo_valor_grado);

                break;

            case PROCESO_ESTADO:
                _imprimir_estados_procesos();

                break;
            default:
                printf("El comando que ingresaste no es valido\n");
                break;
            }

        free(array_del_comando);
        caracteres_leidos = getline(&leido, &length, archivo);
    }

    free(leido);
    fclose(archivo);
    free(path_codigo);

    retomar_planificacion();
}

// ------------------------ FUNCIONES AUXILIARES

int _asignar_PID(void){
    return pid_nuevo_proceso++;
}//Lo uso y lo incremento para que otro PCB tenga un pid distinto.

// CAPAZ ESTAS 3 FUNCIONES DEBERIAN IR EN colasEstados.c
void imprimir_cola(t_estado *estado) {
    list_iterate(estado->cola, _mostrar_pcbs);
}

void _mostrar_pcbs(void *pcbDeLista) {
    t_pcb *pcb = (t_pcb *)pcbDeLista;
    printf("\t\tPID: %d\n", pcb->pid);
}

void imprimir_cola_bloqueados(void){

    t_list* interfaces = dictionary_elements(interfaces_conectadas); // Lista de elementos de tipo t_interfaz*
    int cant_interfaces = list_size(interfaces);

    for (int i = 0; i < cant_interfaces; i++)
    {
        t_interfaz *interfaz = list_remove(interfaces, 0);
        printf("\t%s:\n", interfaz->nombre);
        imprimir_cola(interfaz->bloqueados);
    }

}

int _esta_ejecutando(int pid_buscado){
    int size = list_size(exec->cola);
    int encontro = 0;

    for (int i = 0; i < size; i++) {
            pthread_mutex_lock(&(exec->mutex_cola));
                t_pcb *pcb = list_get(exec->cola, i);
            pthread_mutex_unlock(&(exec->mutex_cola));

            if (pcb->pid == pid_buscado)
            {   
                free(pcb);
                encontro = 1;
                break;
            }
        }
    return encontro;
}
