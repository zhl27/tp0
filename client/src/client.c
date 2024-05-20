#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger(); // ya está declarada como global en el utils

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger, "El valor de ip es: %s", ip);
	log_info(logger, "El valor de puerto es: %s", puerto);
	log_info(logger, "El valor de valor es: %s", valor);

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	int cod_op = recibir_operacion(conexion); // en este caso no nos importa saber el cod_op pero usamos la funcion esta para reciclar código.
	recibir_mensaje(conexion);
	
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "TP0", true, 2); 

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if (nuevo_config == NULL) {
		exit(0);
	}

	return nuevo_config;
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	printf("Introducir textos a enviar. Deje vacio para enviar todas las lineas.\n");

	// Leemos y esta vez agregamos las lineas al paquete
	while (true) {
		leido = readline("> ");
		if (!strcmp(leido, "")) {
			break; // salimos del loop
		}
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		log_info(logger, "> %s", leido);
	}

	// Enviamos el paquete
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
