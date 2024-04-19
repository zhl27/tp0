#include "server.h"

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir clientes");
	
	// multiplexación
	while(1){
		int cliente_fd = esperar_cliente(server_fd);
		pthread_t hilo_cliente;
		pthread_create(&hilo_cliente,NULL,atender_cliente,cliente_fd);
		pthread_detach(hilo_cliente);
	}

	return EXIT_SUCCESS;
}



void *atender_cliente(int cliente_fd){
	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
			case MENSAJE:
				recibir_mensaje(cliente_fd);
				enviar_mensaje("Respuesta del server!!!",cliente_fd);
				break;
			case PAQUETE:
				lista = recibir_paquete(cliente_fd);
				log_info(logger, "Me llegaron los siguientes valores:");//recordar que el logger es una variable global
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
	return NULL;
}


void iterator(char* value) {
	log_info(logger,"%s", value);
}
