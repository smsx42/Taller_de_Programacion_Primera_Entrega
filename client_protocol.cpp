#include <string>
#include <vector>
#include <iostream>
#include "client_protocol.h"
#include "constantes.h"

std::string Client_protocol::devolver_hexadecimal_caracter(
std::vector<char> &vector_hexadecimal) {
    
    std::stringstream cadena_ascii;
    for (size_t i = 0; i < vector_hexadecimal.size(); i += 1) {
        std::string byte;
        byte = static_cast<char>(vector_hexadecimal[i]);
        cadena_ascii << byte;
    }

    return cadena_ascii.str();
}

char Client_protocol::devolver_accion_byte(const std::string &accion) {
    
    char tranformado = BYTE_FIN;
    if (accion == ACCION_JUMP) {
        tranformado = BYTE_JUMP;
    } else if (accion == ACCION_RIGHT) {
        tranformado = BYTE_RIGHT;
    } else if (accion == ACCION_LEFT) {
        tranformado = BYTE_LEFT;
    } else if (accion == ACCION_DUCK) {
        tranformado = BYTE_DUCK;
    } else if (accion == ACCION_HIT) {
        tranformado = BYTE_HIT;
    }

    return tranformado;
}

std::vector<char> Client_protocol::devolver_formato_enviable(
std::string &string_linea) {
    
    std::stringstream string_espaciado(string_linea);
    std::string palabra;
    std::vector<char> vector_bytes;
    while (string_espaciado >> palabra) {
        vector_bytes.push_back(devolver_accion_byte(palabra));
    }
    vector_bytes.push_back(BYTE_FIN);
    return vector_bytes;
}

Client_protocol::Client_protocol(const std::string &hostname,
const std::string &nombre_servidor) : hostname(hostname),
socket(hostname.c_str(), nombre_servidor.c_str()) {}

void Client_protocol::enviar(const std::vector<char> &vector_bytes) {
    
    bool was_closed = false;
    auto buf = vector_bytes;
    this->socket.sendall(buf.data(), buf.size(), &was_closed);

}

int Client_protocol::devolver_bytes_decimal(unsigned char buf_bytes[2]) {

    std::stringstream numero_hexadecimal;
    numero_hexadecimal << std::hex << std::setw(2) << 
    std::setfill('0') << static_cast<int> (buf_bytes[0]);
    numero_hexadecimal << std::hex << std::setw(2) << 
    std::setfill('0') << static_cast<int> (buf_bytes[1]);
    std::string bytes_concatenados =  numero_hexadecimal.str();
    std::istringstream iss(bytes_concatenados);
    int numero_decimal = 0;
    iss >> std::hex >> numero_decimal;

    return numero_decimal;
}

std::vector<char> Client_protocol::esperar_resupuesta() {
    bool was_closed = false;
    unsigned char buf_bytes_iniciales[2];
    // Yo se que siempre necesito los dos primeros
    // bytes enivados por el servidor.
    this->socket.recvall(buf_bytes_iniciales, 2, &was_closed);
    int cantidad_bytes = devolver_bytes_decimal(buf_bytes_iniciales);
    std::vector<char> respuesta;
    respuesta.resize(cantidad_bytes);
    // Yo se que siempre el resto del mensaje son
    // los dos primeros bytes concatenados en decimal.
    this->socket.recvall(respuesta.data(), respuesta.size(), &was_closed);
    return respuesta;
}
