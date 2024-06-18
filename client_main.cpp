#include <iostream>
#include <vector>

#include "socket.h"
#include "client_protocol.h"
#include "constantes.h"

#define ERROR 0

int main(int argc, char *argv[]) {
    
    char *nombre_cliente;
    char *nombre_servidor;
    if (argc == 3) {
        nombre_cliente = argv[1];
        nombre_servidor = argv[2];

    } else {
        std::cout << "Hubo un error con el cliente." << "\n";
        return ERROR;
    }

    Client_protocol protocolo(nombre_cliente, nombre_servidor);
    std::string entrada_usuario;
    std::vector<std::string> vector_entradas_usuario;
    while (std::getline(std::cin, entrada_usuario)) {
        vector_entradas_usuario.push_back(entrada_usuario);
    }
    
    for (size_t i = 0; i < vector_entradas_usuario.size(); i++) {
        std::vector<char> vector_bytes =
        protocolo.devolver_formato_enviable(vector_entradas_usuario[i]);
        protocolo.enviar(vector_bytes);
        std::vector<char> respuesta =
        protocolo.esperar_resupuesta();
        std::string string_imprimir =
        protocolo.devolver_hexadecimal_caracter(respuesta);
        std::cout << string_imprimir  << std::endl;
    }

    return 0;
}
