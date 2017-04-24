/*
 * study class layout
 * time: 2017/04/21
 */
#pragma once

#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// Class Layout
typedef void(*Fun)(void);
// myself
namespace SelfStudy{
	class Base {
	public:
		virtual void f() { cout << "Base::f" << endl; }
		virtual void g() { cout << "Base::g" << endl; }
		virtual void h() { cout << "Base::h" << endl; }

		int b;
	};
	class Derive : public Base {
	public:
		virtual void f1() { cout << "Derive::f1" << endl; }
		virtual void g1() { cout << "Derive::g1" << endl; }
		virtual void h1() { cout << "Derive::h1" << endl; }

		int d;
	};

	class Base1 {
	public:
		virtual void f() { cout << "Base1::f" << endl; }
		virtual void g() { cout << "Base1::g" << endl; }
		virtual void h() { cout << "Base1::h" << endl; }

		int b1;
	};
	class Base2 {
	public:
		virtual void f() { cout << "Base2::f" << endl; }
		virtual void g() { cout << "Base2::g" << endl; }
		virtual void h() { cout << "Base2::h" << endl; }

		int b2;
	};
	class Base3 {
	public:
		virtual void f() { cout << "Bas3::f" << endl; }
		virtual void g() { cout << "Base3::g" << endl; }
		virtual void h() { cout << "Base3::h" << endl; }

		int b3;
	};
	class Derive1 : public Base1, public Base2, public Base3 {
	public:
		virtual void f1() { cout << "Derive1::f1" << endl; }
		virtual void g1() { cout << "Derive1::g1" << endl; }
		virtual void h1() { cout << "Derive1::h1" << endl; }

		int d;
	};
}

namespace SelfVirtual{
	class B
	{
	public:
		int ib;
		char cb;
	public:
		B():ib(0),cb('B') {}

		virtual void f() { cout << "B::f()" << endl;}
		virtual void Bf() { cout << "B::Bf()" << endl;}
	};
	class B1 : virtual public B
	{
	public:
		int ib1;
		char cb1;
	public:
		B1():ib1(11),cb1('1') {}

		virtual void f() { cout << "B1::f()" << endl;}
		virtual void f1() { cout << "B1::f1()" << endl;}
		virtual void Bf1() { cout << "B1::Bf1()" << endl;}

	};
	class B2: virtual public B
	{
	public:
		int ib2;
		char cb2;
	public:
		B2():ib2(12),cb2('2') {}

		virtual void f() { cout << "B2::f()" << endl;}
		virtual void f2() { cout << "B2::f2()" << endl;}
		virtual void Bf2() { cout << "B2::Bf2()" << endl;}

	};

	class D : public B1, public B2
	{
	public:
		int id;
		char cd;
	public:
		D():id(100),cd('D') {}

		virtual void f() { cout << "D::f()" << endl;}
		virtual void f1() { cout << "D::f1()" << endl;}
		virtual void f2() { cout << "D::f2()" << endl;}
		virtual void Df() { cout << "D::Df()" << endl;}

	};
}

// InheritanceWithoutPolymorphism
namespace InheritanceWithoutPolymorphism{
	class Point2d{
	public:
		Point2d(float x = 0.0, float y = 0.0)
			: _x(x), _y(y){};
		float x() const {return _x;}
		float y() const {return _y;}

		void x(float newX) {_x = newX;}
		void y(float newY) {_y = newY;}

		void operator += (const Point2d& rhs){
			_x += rhs.x();
			_y += rhs.y();
		}
	protected:
		float _x, _y;
	};

	class Point3d : public Point2d{
	public:
		Point3d(float x = 0.0, float y = 0.0, float z = 0.0)
			: Point2d(x, y), _z(z){};

		float z() const {return _z;}
		void z(float newZ) {_z = newZ;}

		void operator += (const Point3d& rhs){
			Point2d::operator+=(rhs);
			_z += rhs.z();
		}
	protected:
		float _z;
	};

} //InheritanceWithoutPolymorphism


// 加上多态：AddingPolymorphism
namespace AddingPolymorphism{
	class Point2d{
	public:
		Point2d(float x = 0.0, float y = 0.0)
			: _x(x), _y(y){};
		float x() const {return _x;}
		float y() const {return _y;}
		virtual float z() const {return 0.0;}

		void x(float newX) {_x = newX;}
		void y(float newY) {_y = newY;}
		virtual void z(float) {}

		virtual void operator += (const Point2d& rhs){
			_x += rhs.x();
			_y += rhs.y();
		}
	protected:
		float _x, _y;
	};

	class Point3d : public Point2d{
	public:
		Point3d(float x = 0.0, float y = 0.0, float z = 0.0)
			: Point2d(x, y), _z(z){};

		float z() const {return _z;}
		void z(float newZ) {_z = newZ;}

		void operator += (const Point3d& rhs){
			Point2d::operator+=(rhs);
			_z += rhs.z();
		}
	protected:
		float _z;
	};

} //AddingPolymorphism

// 3.多重继承：MultipleInheritance
namespace MultipleInheritance{
	class Point2d{
	public:
		Point2d(float x = 0.0, float y = 0.0)
			: _x(x), _y(y){};
		float x() const {return _x;}
		float y() const {return _y;}
		virtual float z() const {return 0.0;}

		void x(float newX) {_x = newX;}
		void y(float newY) {_y = newY;}
		virtual void z(float) {}

		virtual void operator += (const Point2d& rhs){
			_x += rhs.x();
			_y += rhs.y();
		}
	protected:
		float _x, _y;
	};

	class Point3d : public Point2d{
	public:
		Point3d(float x = 0.0, float y = 0.0, float z = 0.0)
			: Point2d(x, y), _z(z){};

		float z() const {return _z;}
		void z(float newZ) {_z = newZ;}

		void operator += (const Point3d& rhs){
			Point2d::operator+=(rhs);
			_z += rhs.z();
		}
	protected:
		float _z;
	};

	class Vertex 
	{
	public:
		//
		virtual Vertex* GetNext() {return next;};
	protected:
		Vertex *next;
	};

	class Vertex3d :
		public Point3d, public Vertex{
	public:
		// ...
		
	protected:
		float mumble;
	};

} //MultipleInheritance

// Virtua Inheritance
namespace VirtuaInheritance{
	class Point2d{
	public:
		Point2d(float x = 0.0, float y = 0.0)
			: _x(x), _y(y){};
		float x() const {return _x;}
		float y() const {return _y;}
		virtual float z() const {return 0.0;}

		void x(float newX) {_x = newX;}
		void y(float newY) {_y = newY;}
		virtual void z(float) {}

		virtual void operator += (const Point2d& rhs){
			_x += rhs.x();
			_y += rhs.y();
		}
	protected:
		float _x, _y;
	};

	class Point3d : public virtual Point2d{
	public:
		Point3d(float x = 0.0, float y = 0.0, float z = 0.0)
			: Point2d(x, y), _z(z){};

		float z() const {return _z;}
		void z(float newZ) {_z = newZ;}

		void operator += (const Point3d& rhs){
			Point2d::operator+=(rhs);
			_z += rhs.z();
		}
	protected:
		float _z;
	};

	class Vertex : public virtual Point2d
	{
	public:
		//virtual Vertex* GetNext() {return next;};
	protected:
		Vertex *next;
	};

	class Vertex3d :
		public Vertex, public Point3d {
	public:
		// ...

	protected:
		float mumble;
	};

} //Virtua Inheritance
