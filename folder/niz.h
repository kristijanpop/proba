
#include <stdlib.h>

class intArray{
    private:
    int length;
    int capacity;
    int* array;
    public:
    intArray();
    ~intArray();
    void apeend(int value);
    int getLength() const ;

    int at(int indeks) const;


};

intArray::intArray(){

    this->length=0;
    this->capacity=1;
    this->array=(int*)malloc(sizeof(int)* this->capacity);




}

intArray::~intArray(){


delete [] this->array ;


}
void  intArray::apeend(int value ){


    if(this->capacity==this->length )
    {
        this->capacity*=2;
        this->array=(int *)realloc(this->array,sizeof(int)* this->capacity);
    }

    this->array[this->length]=value;

    this->length++;
}

int intArray::getLength()const {



    return this->length;

}

int intArray::at(int indeks)const{


        return this->array[indeks];





}