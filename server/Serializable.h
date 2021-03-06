#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <stdlib.h>
#include <string>
#include "json.hpp"
using json = nlohmann::json;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Este interfaz debe implementarlo aquellos objetos que se transmitan por red
 *  Define una serialización sencilla para la transmisión:
 *
 *  +--------+
 *  |        |  Esta es la representación binaria del objeto. Cada objeto
 *  |        |  deberá implementar dos métodos:
 *  |  data  |    - to_bin() para rellenar
 *  |        |    - from_bin() para reconstruirse a partir de data.
 *  |        |    - size es el tamaño total de la región data
 *  +--------+
 */
class Serializable
{
public:
    Serializable(){ };

    virtual ~Serializable()=default;

    /**
     *  Genera la representación binaria de la clase. Debe inicializar
     *  el buffer interno con la función helper alloc_data.
     */
    virtual void to_bin() = 0;

    /**
     *  Esta función recibe un objeto serializado y lo reconstruye.
     *    @param data representación binaria del objeto
     *    @return 0 si éxito -1 en caso contrario
     */
    virtual int from_bin(json data) = 0;
    /**
     *  Devuelve un puntero al buffer interno con la representación del objeto.
     *  Debe inicializarse previamente via Serializable::to_bin()
     *    @return objeto serializado
     */

    /**
     * Devuelve un puntero al mensaje de protobuf
     * @return mensaje 
     */
    std::string data()
    {
        return _data;
    }

    /**
     *  @return tamaño del objeto serializado
     */
    int32_t size()
    {
        return _size;
    }

    json getJSON()const{return _json;}

protected:
    std::string _data;

    int32_t _size;

    json _json;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#endif /* SERIALIZABLE_H_ */
