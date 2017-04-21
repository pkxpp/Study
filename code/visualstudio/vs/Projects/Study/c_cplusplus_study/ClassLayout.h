/*
 * study class layout
 * time: 2017/04/21
 */
#pragma once

#include <iostream>
using namespace std;

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

	class Vertex : public virtual Point2d
	{

	};

} //InheritanceWithoutPolymorphism
