#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include "socket.h"

class Client_protocol {
 private:
  
  const std::string hostname;
  Socket socket;
  
  // Pre: Recibe un buf con dos bytes.
  // Pos: Concatena los dos bytes del buf y los convierte en 
  // decimal, y devuelve el valor.
  int devolver_bytes_decimal(unsigned char buf_bytes[2]);
    
 public:
  
  // Constructor de Protocol.
  explicit Client_protocol(const std::string &hostname,
  const std::string &nombre_servidor);
  
  // Pre: Recibe un string que representa una accion posible.
  // Pos: Devuelve el byte que representa la accion posible.
  char devolver_accion_byte(const std::string &accion);
  
  // Pre: Recibe un string con acciones separadas por espacios.
  // Pos: Devuelve un vector de bytes que representan todas las acciones
  // ingresadas por el usuario.
  std::vector<char> devolver_formato_enviable(
  std::string &string_linea);
  
  // Pre: Recibe un vector de bytes en hexadecimal.
  // Pos: Asocia el byte hexadecimal a su equivalente en codigo ASCII.
  std::string devolver_hexadecimal_caracter(
  std::vector<char> &cadena_hexadecimal);
  
  // Pre: Recibe un vector de bytes que representa la acciones del usuario.
  // Pos: Envia al servidor el vector de bytes.
  void enviar(const std::vector<char> &vector_bytes);
  
  // Pre: -
  // Pos: Espera la respuesta del servidor y envia el mensaje al usuario.
  std::vector<char> esperar_resupuesta();
};

#endif  // CLIENT_PROTOCOL_H
