int main(){
    int main(void) {
    	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

    	int server_fd = iniciar_servidor();

    	log_info(logger, "CPU listo");
    	int cliente_fd = esperar_cliente(server_fd);
        
    	t_list* lista;
    	while (1) {
    		int cod_op = recibir_operacion(cliente_fd);
    		switch (cod_op) {
    		case MENSAJE:
    			recibir_mensaje(cliente_fd);
    			break;
    		case PAQUETE:
    			lista = recibir_paquete(cliente_fd);
    			log_info(logger, "Me llegaron los siguientes valores:\n");
    			list_iterate(lista, (void*) iterator);
    			break;
    		case -1:
    			log_error(logger, "el cliente se desconecto. Terminando servidor");
    			return EXIT_FAILURE;
    		default:
    			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
    			break;
    		}
    	}
    	return EXIT_SUCCESS;
    }//

void iterator(char* value) {
	log_info(logger,"%s", value);
}


}

int iniciar_servidor(void)
{
	int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "4444", &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family,
                        	servinfo->ai_socktype,
                        	servinfo->ai_protocol);
	// Asociamos el socket a un puerto
	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}