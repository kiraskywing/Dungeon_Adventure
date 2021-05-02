#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Object {
private:
    string name, tag;
public:
    Object(string nm="", string tg="") : name(nm), tag(tg) {}
    virtual ~Object() {}

    /* pure virtual function */
    virtual bool triggerEvent(Object*) = 0;

    /* Set & Get function*/
    void setName(string nm) { name = nm; }
    void setTag(string tg) { tag = tg; }
    string getName() { return name; }
    string getTag() { return tag; }
};

int inputOptimizer(const int, string mode="");

#endif // OBJECT_H_INCLUDED