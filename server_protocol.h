#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <iostream>
#include <string>
#include <vector>

#include "socket.h"

class Server_protocol {
 private:

  Socket peer;
  bool was_closed;

  // Pre: Recibe el vector de acciones original y el vactor de
  // acciones con combos con sus iteradores.
  // Pos: Completa el vector de acciones con los combos disponibles
  // o las acciones originales en caso de que no sean combos.
  void verificar_algun_combo(std::vector<char> const &vector_acciones_basicas,
  std::vector<char> &vector_acciones_totales,
  size_t &iterador, size_t &iterador_aux);

  // Pre: Recibe el vector de acciones totales y elimina las acciones que
  // forman un combo, agregando el mismo en las posiciones del iterador aux.
  // Pos: Modifica el vector de acciones totales agregando los combos.
  void agregar_byte_combo(std::vector<char> &vector_acciones_totales,
  size_t const &iterador_aux, uint8_t const &byte, size_t const &cantidad);
  
  // Pre: Recibe un byte que representa una accion basica o un combo.
  // Pos: Devuelve una cadena asociada auna accion bascia o un combo.
  std::string devolver_bytes_accion(char const &byte);
  
  // Pre: Recibe un vector de caracteres que representan acciones y un iterador.
  // Pos: Verifica si la accion de Uppercut y devuelve un bool.
  bool verificar_byte_uppercut(std::vector<char> const &vector_acciones_basicas, 
  size_t const &iterador);

  // Pre: Recibe un vector de caracteres que representan acciones y un iterador.
  // Pos: Verifica si la accion de Uppercut y devuelve un bool.
  bool verificar_byte_sidekick(std::vector<char> const &vector_acciones_basicas, 
  size_t const &iterador);

  // Pre: Recibe un vector de caracteres que representan acciones y un iterador.
  // Pos: Verifica si la accion de Uppercut y devuelve un bool.
  bool verificar_byte_highkick(std::vector<char> const &vector_acciones_basicas, 
  size_t const &iterador);

 public:
  
  // Constructor de Protocol.
  explicit Server_protocol(Socket&& socket);
  
  // Pre: Recibe un socket y un bool que indica si se cerro el mismo.
  // Pos: Devuelve el mensaje completo enviado por el usuario en bytes.
  std::vector<char> devolver_mensaje_recibido();
  
  // Pre: Recibe un vector con bytes que representan las acciones basicas.
  // Pos: Devuelve un vector con bytes que representan las acciones bascias
  // y los combos.
  std::vector<char> devolver_acciones_totales(
  std::vector<char> vector_acciones_basicas);
  
  // Pre: Recibe un vector de bytes que representan las acciones bascias
  // y los combos.
  // Pos: Devuelve un vector de bytes en formato hexadecimal que representan
  // las acciones bascias y los combos.
  std::vector<unsigned char> devolver_mensaje_hexadecimal(
  std::vector<char> &vector_acciones_combos);
  
  // Pre: Recibe el socket intermediario junto con
  // el mensaje hexadecimal a enviar en forma
  // de vector de bytes y un indicado booleano.
  // Pos:Envia el mensaje al usuario.
  void enviar(std::vector<unsigned char> &vector_hexadecimal);

  // Pre: -
  // Pios: Retorna was_closed.
  bool get_was_closed();
};

#endif  // SERVER_PROTOCOL_H
