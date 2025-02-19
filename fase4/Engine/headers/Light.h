#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "../../Utils/point.h"

#include <vector>
#include <iostream>


class Light {
    private:
        std::string type;
        std::vector<float> pos;
        std::vector<float> dir;
        float cutoff;
        friend std::ostream& operator<<(std::ostream& os, const Light& light);

    public:
        // Construtores
        Light();

        // Setters
        void setPosition(Point& pos);
        void setDirection(Point& direction);
        void setCutoff(float cutoff);
        void setType(const std::string& type);

        // Getters
        std::vector<float> getPosition() const;
        std::vector<float> getDirection() const;
        size_t getCutoff() const;
        std::string getType() const;

        // std::ostream& operator<<(std::ostream& os, const std::vector<float>& vec);
        // std::ostream& operator<<(std::ostream& os, const Light& light);

}; 
