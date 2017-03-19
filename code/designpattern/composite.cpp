#include <iostream>
#include <vector>
#include <unistd.h>
#include <time.h>

using namespace std;

class Node{
public:
	Node(){}
	Node(char* name){_name = name;}
	virtual void Draw(){};
	virtual void Add(Node* child){};
	char* GetName(){return _name;};
private:
	char* _name;
};

class Label: public Node{
public:
	Label(char* name):Node(name){}
	void Draw(){
		cout << "Draw image: " << GetName() << endl;
	}
};

class Sprite: public Node{
public:
	Sprite(char* name):Node(name){}
	void Draw(){
		cout << "Draw Sprite." << GetName() << endl;
	}
};

class Layer: public Node{
public:
	void Add(Node* child){
		_children.push_back(child);
	};

	void Draw(){
		cout << "Draw Layer begin ... " << endl;
		DrawChildren();
		cout << "Draw Layer end. " << endl;
	}
	void DrawChildren(){
		vector<Node*>::iterator it = _children.begin();
		for (; it != _children.end(); ++it)
			(*it)->Draw();
	}

private:

	vector<Node*> _children;
};

class Scene: public Layer{
public:
	void Draw(){
		cout << "Draw Scene begin ... " << endl;
		DrawChildren();
		cout << "Draw Scene end. " << endl;
	}
};

class Director{
public:
	Director(Scene* s){
		_scene = s;
	}

	void Rend(){
		if (_scene)
			_scene->Draw();
	}
private:
	Scene* _scene;
};

int main(){
	const int WAIT_TIME = 2;
	Scene* pScene = new Scene();
	pScene->Add(new Label("Label 1"));
	pScene->Add(new Sprite("Sprite 1"));
	Layer* pLayer = new Layer();
	pLayer->Add(new Label("Label 2 on Layer"));
	pLayer->Add(new Sprite("Sprite 2 on Layer"));
	pScene->Add(pLayer);
	Director director(pScene);
	time_t tLast; 
	while(true){
		tLast = time(NULL);
		director.Rend();
		
		time_t tNow = time(NULL);
		if(tNow - tLast < WAIT_TIME){
			//seconds = 1000 * 1000 micro seconds 
			usleep((WAIT_TIME - tNow + tLast)*1000*1000);
		}
	}
}
