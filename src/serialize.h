#ifndef FRUITSHARK_SERIALIZE_H
#define FRUITSHARK_SERIALIZE_H


class Serializable {
public:
    [[nodiscard]] virtual size_t size() const = 0;

    virtual void write(unsigned char* buf) const = 0;

    virtual void read(const unsigned char* buf) = 0;
};


#endif //FRUITSHARK_SERIALIZE_H
