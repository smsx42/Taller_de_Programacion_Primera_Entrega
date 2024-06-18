#include <vector>

#include "socket.h"
#include "constantes.h"
#include "server_protocol.h"

#define ERROR -1

int main(int argc, char *argv[]) {
    
    char *nombre_servidor;
    if (argc == 2) {
        nombre_servidor = argv[1];
    } else {
        std::cout << "Hubo un error." << "\n";
        return ERROR;
    }
    
    Socket srv(nombre_servidor);
    Server_protocol protocolo(srv.accept());
    int contador_acciones = 0;
    while (!protocolo.get_was_closed()) {
        std::vector<char> vector_acciones_basicas =
        protocolo.devolver_mensaje_recibido();

        if (protocolo.get_was_closed()) {
            break;
        }
        std::vector<char> vector_acciones_totales =
        protocolo.devolver_acciones_totales(vector_acciones_basicas);
        contador_acciones += (vector_acciones_totales.size() - 1);

        std::vector<unsigned char> vector_hexadecimal =
        protocolo.devolver_mensaje_hexadecimal(vector_acciones_totales);
        protocolo.enviar(vector_hexadecimal);
    }

    std::cout << "Actions: " << contador_acciones << std::endl;
    srv.close();

    return 0;
}
