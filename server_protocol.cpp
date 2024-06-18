#include <string>
#include <utility>
#include <sstream>
#include <iomanip>

#include "server_protocol.h"
#include "constantes.h"

Server_protocol::Server_protocol(Socket &&socket) : peer(std::move(socket)), was_closed(false) {}

bool Server_protocol::get_was_closed() {
    
    return was_closed;
}

std::vector<char> Server_protocol::devolver_mensaje_recibido() {
    
    char buf_recibido;
    bool termino_recv = false;
    std::vector<char> vector_acciones_basicas;
    // Recibe un byte hasta encontrar el byte de fin.
    while (!termino_recv) {
        
        this->peer.recvall(&buf_recibido, 1, &this->was_closed);
        vector_acciones_basicas.push_back(buf_recibido);
        if (buf_recibido == BYTE_FIN) {
            termino_recv = true;
        }
    }

    return vector_acciones_basicas;
}

void Server_protocol::agregar_byte_combo(std::vector<char> &vector_acciones_totales,
size_t const &iterador_aux,uint8_t const &byte, size_t const &cantidad) {
    
    for (size_t i = 0; i <= cantidad; i++) {
        vector_acciones_totales.erase(vector_acciones_totales.begin() +
        iterador_aux);
    }

    vector_acciones_totales.insert(vector_acciones_totales.begin()
    + iterador_aux, byte);
}

bool Server_protocol::verificar_byte_uppercut(std::vector<char> const &vector_acciones_basicas, 
size_t const &iterador) {
    
    return (vector_acciones_basicas[iterador] == BYTE_JUMP
    && vector_acciones_basicas[iterador + 1] == BYTE_JUMP
    && vector_acciones_basicas[iterador + 2] == BYTE_HIT);
}

bool Server_protocol::verificar_byte_sidekick(std::vector<char> const &vector_acciones_basicas, 
size_t const &iterador) {
    
    return(vector_acciones_basicas[iterador] == BYTE_HIT
    && vector_acciones_basicas[iterador + 1] == BYTE_DUCK
    && vector_acciones_basicas[iterador + 2] == BYTE_LEFT);
}

bool Server_protocol::verificar_byte_highkick(std::vector<char> const &vector_acciones_basicas, 
size_t const &iterador) {
    return(vector_acciones_basicas[iterador] == BYTE_LEFT
    && vector_acciones_basicas[iterador + 1] == BYTE_RIGHT
    && vector_acciones_basicas[iterador + 2] == BYTE_JUMP
    && vector_acciones_basicas[iterador + 3] == BYTE_HIT);
}

void Server_protocol::verificar_algun_combo(
std::vector<char> const &vector_acciones_basicas,
std::vector<char> &vector_acciones_totales,
size_t &iterador, size_t &iterador_aux) {
    
    if (verificar_byte_uppercut(vector_acciones_basicas, iterador)) {
        agregar_byte_combo(vector_acciones_totales,
        iterador_aux, BYTE_UPPERCUT, TOPE_UPPERCUT);
        iterador += TOPE_UPPERCUT;
    } else if (verificar_byte_sidekick(vector_acciones_basicas, iterador)) {
        agregar_byte_combo(vector_acciones_totales,
        iterador_aux, BYTE_SIDEKICK, TOPE_SIDEKICK);
        iterador += TOPE_SIDEKICK;
    } else if (verificar_byte_highkick(vector_acciones_basicas, iterador)) {
        agregar_byte_combo(vector_acciones_totales,
        iterador_aux, BYTE_HIGHKICK, TOPE_HIGHKICK);
        iterador += TOPE_HIGHKICK;
    }

    iterador_aux += 1;
}

std::vector<char> Server_protocol::devolver_acciones_totales
(std::vector<char> vector_acciones_basicas) {
    
    std::vector<char> vector_acciones_totales;
    for (size_t i = 0; i < vector_acciones_basicas.size(); i++) {
        vector_acciones_totales.push_back(vector_acciones_basicas[i]);
    }
    size_t iterador_auxiliar = 0;
    for (size_t i = 0; i < vector_acciones_basicas.size(); i++) {
        if ((i + 2) < vector_acciones_basicas.size()
        || (i + 3) < vector_acciones_basicas.size()) {
            verificar_algun_combo(vector_acciones_basicas,
            vector_acciones_totales, i, iterador_auxiliar);
        }
    }

    return vector_acciones_totales;
}

std::string Server_protocol::devolver_bytes_accion(char const &byte) {
    
    std::string accion;
    if (byte == BYTE_JUMP) {
        accion = ACCION_JUMP;
    } else if (byte == BYTE_RIGHT) {
        accion = ACCION_RIGHT;
    } else if (byte == BYTE_LEFT) {
        accion = ACCION_LEFT;
    } else if (byte == BYTE_DUCK) {
        accion = ACCION_DUCK;
    } else if (byte == BYTE_HIT) {
        accion = ACCION_HIT;
    } else if (byte == BYTE_FIN) {
        accion = ACCION_NOP;
    } else if (byte == BYTE_UPPERCUT) {
        accion = ACCION_UPPERCUT;
    } else if (byte == BYTE_SIDEKICK) {
        accion = ACCION_SIDEKICK;
    } else { accion = ACCION_HIGHKICK; }
    
    return accion;
}

std::vector<unsigned char> Server_protocol::devolver_mensaje_hexadecimal
(std::vector<char> &vector_acciones_combos) {
    std::string string_acciones_totales;

    for (size_t i = 0; i < vector_acciones_combos.size(); i++) {
        string_acciones_totales +=
        devolver_bytes_accion(vector_acciones_combos[i]);
        string_acciones_totales += " ";
    }
    
    // Necesito separar los bytes para poder agregarlos
    // al vector.
    uint16_t bytes_totales = (string_acciones_totales.length() - 2);
    uint8_t primer_byte = (bytes_totales >> 8) & 0XFF;
    uint8_t segundo_byte = bytes_totales & 0XFF;

    std::vector<unsigned char> vector_hexadecimal;
    vector_hexadecimal.push_back(primer_byte);
    vector_hexadecimal.push_back(segundo_byte);

    for (size_t i = 0; i < string_acciones_totales.size(); i++) {
        uint8_t codigo_ascii = static_cast<uint8_t>(string_acciones_totales[i]);
        vector_hexadecimal.push_back((codigo_ascii));
    }
    vector_hexadecimal.pop_back();
    
    return vector_hexadecimal;
}

void Server_protocol::enviar(std::vector<unsigned char> &vector_hexadecimal) {
    
    this->peer.sendall(vector_hexadecimal.data(),
    vector_hexadecimal.size() - 1, &this->was_closed);
}
