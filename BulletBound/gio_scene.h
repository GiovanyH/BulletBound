#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Entity
{
public:
    // model data 
    string object_type;
    string model_path;
    glm::vec3 position;

    unsigned int entityID;

    ~Entity() {};
};

class Scene
{
public:
    // model data 
    vector<Entity> entities;

    void loadModels()
    {
        string path = "resources/scenes/benchmark_scene.txt";

        std::ifstream inputFile(path.c_str()); // Open the file
        if (!inputFile.is_open()) {
            std::cerr << "Error opening the file!" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(inputFile, line)) { // Read each line
            Entity entity;
            std::stringstream ss(line);
            std::string token;
            std::string v;
            std::vector<std::string> tokens;

            while (std::getline(ss, token, ',')) { // Split the line by ,
                std::stringstream positionSS(token.substr(1)); // Create a stringstream for the position token
                glm::vec3 position;
                if (token[0] == 't') entity.object_type = string(token.c_str() + 1);
                if (token[0] == 'p') entity.model_path = string(token.c_str() + 1);
                if (token[0] == 'v')
                {
                    while (std::getline(positionSS, v, ' ')) {
                        if (v[0] == 'x') position.x = stof(string(v.substr(1)));
                        if (v[0] == 'y') position.y = stof(string(v.substr(1)));
                        if (v[0] == 'z') position.z = stof(string(v.substr(1)));
                    }

                    entity.position = position;
                }
            }
            entity.entityID = entities.size();
            entities.push_back(entity);
        }

        inputFile.close(); // Close the file
    }

    ~Scene() {};
};

class Line {
    int shaderProgram;
    unsigned int VBO, VAO;
    vector<float> vertices;
    glm::mat4 MVP;
    glm::vec3 lineColor;
public:
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    Line(glm::vec3 start, glm::vec3 end) {

        startPoint = start;
        endPoint = end;
        lineColor = glm::vec3(1, 1, 1);
        MVP = glm::mat4(1.0f);

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 MVP;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec3 color;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(color, 1.0f);\n"
            "}\n\0";

        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors

        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors

        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        vertices = {
             start.x, start.y, start.z,
             end.x, end.y, end.z,

        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    int setMVP(glm::mat4 mvp) {
        MVP = mvp;
        return 1;
    }

    int setColor(glm::vec3 color) {
        lineColor = color;
        return 1;
    }

    int draw() {
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);

        // update the lines vertices to draw - set it to start and end point
        vertices = {
			 startPoint.x, startPoint.y, startPoint.z,
			 endPoint.x, endPoint.y, endPoint.z,
		};

        // update the lines vertices to draw
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        return 1;
    }

    ~Line() {

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};

class ViewportGizmo
{
    Line *lines[3];
    glm::mat4 MVP;

    public:
        void init() {
			lines[0] = new Line(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
            lines[0]->setColor(glm::vec3(1, 0, 0));
			lines[1] = new Line(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            lines[1]->setColor(glm::vec3(0, 1, 0));
            lines[2] = new Line(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
            lines[2]->setColor(glm::vec3(0, 0, 1));
		}
        int setMVP(glm::mat4 mvp) {
            this->MVP = mvp;
            for (int i = 0; i < 3; i++) {
                lines[i]->setMVP(mvp);
            }
            return 1;
        }

        int draw() {
            for (int i = 0; i < 3; i++) {
				lines[i]->draw();
			}
			return 1;
		}
};

class RayCast
{
    glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 color;
	glm::mat4 MVP;
	Line* line;
	public:
        RayCast(glm::vec3 origin, glm::vec3 direction) {
			this->origin = origin;
			this->direction = direction;
			this->color = glm::vec3(1, 1, 1);
			MVP = glm::mat4(1.0f);
			line = new Line(origin, origin + direction);
		}

        int setMVP(glm::mat4 mvp) {
			MVP = mvp;
			line->setMVP(mvp);
			return 1;
		}

        int setColor(glm::vec3 color) {
			this->color = color;
			line->setColor(color);
			return 1;
		}

        int draw() {
			line->draw();
			return 1;
		}
};

class Triangle3
{
public:
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;

    Line* lines[3];

    glm::mat4 MVP;

    Triangle3(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
		this->a = a;
		this->b = b;
		this->c = c;
		MVP = glm::mat4(1.0f);

		lines[0] = new Line(a, b);
		lines[1] = new Line(b, c);
		lines[2] = new Line(c, a);
	}

    int setMVP(glm::mat4 mvp) {
		MVP = mvp;
        for (int i = 0; i < 3; i++) {
			lines[i]->setMVP(mvp);
		}
		return 1;
	}

    int setColor(glm::vec3 color) {
        for (int i = 0; i < 3; i++) {
			lines[i]->setColor(color);
		}
		return 1;
	}

    int draw() {
        for (int i = 0; i < 3; i++) {
			lines[i]->draw();
		}
		return 1;
	}
};

class LineBox
{
    // Make LineBox from Triangle3
    glm::mat4 MVP;
    glm::vec3 color;

    glm::vec3 min;
    glm::vec3 max;

    public:
        glm::vec3 position;
        Triangle3* triangles[12];
        unsigned int objectID;
        bool show;
        LineBox(glm::vec3 min, glm::vec3 max, unsigned int entityID) {
            this->min = min;
            this->max = max;
			MVP = glm::mat4(1.0f);
			color = glm::vec3(1, 1, 1);

			glm::vec3 a = glm::vec3(min.x, min.y, min.z);
			glm::vec3 b = glm::vec3(max.x, min.y, min.z);
			glm::vec3 c = glm::vec3(max.x, max.y, min.z);
			glm::vec3 d = glm::vec3(min.x, max.y, min.z);
			glm::vec3 e = glm::vec3(min.x, min.y, max.z);
			glm::vec3 f = glm::vec3(max.x, min.y, max.z);
			glm::vec3 g = glm::vec3(max.x, max.y, max.z);
			glm::vec3 h = glm::vec3(min.x, max.y, max.z);

			triangles[0] = new Triangle3(a, b, c);
			triangles[1] = new Triangle3(a, c, d);
			triangles[2] = new Triangle3(e, f, g);
			triangles[3] = new Triangle3(e, g, h);
			triangles[4] = new Triangle3(a, e, f);
			triangles[5] = new Triangle3(a, f, b);
			triangles[6] = new Triangle3(b, f, g);
			triangles[7] = new Triangle3(b, g, c);
			triangles[8] = new Triangle3(c, g, h);
			triangles[9] = new Triangle3(c, h, d);
			triangles[10] = new Triangle3(d, h, e);
			triangles[11] = new Triangle3(d, e, a);

            objectID = entityID;

            show = false;
		}
        // Check box collision with intersection_point using min and max plus the position
        bool checkCollision(glm::vec3 intersection_point) {
            if (intersection_point.x > min.x + position.x && intersection_point.x < max.x + position.x &&
                intersection_point.y > min.y + position.y && intersection_point.y < max.y + position.y &&
                intersection_point.z > min.z + position.z && intersection_point.z < max.z + position.z) {
				return true;
			}
			return false;
		}

        int setMVP(glm::mat4 mvp) {
			MVP = mvp;
            for (int i = 0; i < 12; i++) {
				triangles[i]->setMVP(mvp);
			}
			return 1;
		}

        int setColor(glm::vec3 color) {
			this->color = color;
            for (int i = 0; i < 12; i++) {
				triangles[i]->setColor(color);
			}
			return 1;
		}

        int draw() {
            if (!show) return 0;
            for (int i = 0; i < 12; i++) {
				triangles[i]->draw();
			}
			return 1;
        }
};

#endif