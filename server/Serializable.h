#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <google/protobuf/any.h>
#include <stdlib.h>
#include <google/protobuf/message.h>

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
template <class T>
class Serializable
{
	typedef typename std::enable_if<std::is_base_of<google::protobuf::Message, T>::value>::type check;
public:

    Serializable(){_message = new T();};

    virtual ~Serializable(){
	    //TODO investigar por que da error si hacemos
	    // delete _message
	    // Explota cuando snake intenta liberar la memoria de sus Vec2
    };

    /**
     *  Genera la representación binaria de la clase. Debe inicializar
     *  el buffer interno con la función helper alloc_data.
     */
    virtual void to_bin(){
	fill_message(_message);
	_data.clear();
	static_cast<google::protobuf::Message*>(_message)->AppendToString(&_data);
	_size = _data.size();
    }

    /**
     *  Esta función recibe un objeto serializado y lo reconstruye.
     *    @param data representación binaria del objeto
     *    @return 0 si éxito -1 en caso contrario
     */
    virtual int from_bin(char * data){
	    static_cast<google::protobuf::Message*>(_message)->ParseFromString(data);
	    from_message(_message);
	    return 0;
    };

    /**
     * Rellena el mensaje con la informacion necesaria
     */
    virtual void fill_message(T* msg) = 0;


    /**
     *
     */
    virtual int from_message(T* msg) = 0;
    /**
     *  Devuelve un puntero al buffer interno con la representación del objeto.
     *  Debe inicializarse previamente via Serializable::to_bin()
     *    @return objeto serializado
     */
    
    /**
     * Devuelve un puntero al mensaje de protobuf
     * @return mensaje 
     */
    T* get_message(){
	    return _message;
    }
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

protected:
    std::string _data;

    int32_t _size;
    
    T* _message;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#endif /* SERIALIZABLE_H_ */
