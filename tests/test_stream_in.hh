
#include <cxxtest/TestSuite.h>
#include "bson/bson_stream.hh"
using namespace mongo;

class test {
	public:
		double a;
		double b;
		test() {};
		test( double a, double b ) : a(a), b(b) {}

		friend void operator>>( const mongo::BSONElement &bel, test &t ) {
			bel["a"] >> t.a;
			bel["b"] >> t.b;
		} 

		friend mongo::BSONObjBuilder &operator<<( 
				mongo::BSONObjBuilder &bbuild, const test &t ) { 
			bbuild << "a" << t.a; 
			bbuild << "b" << t.b;
			return bbuild;
		}
};



class TestIn : public CxxTest::TestSuite {
	public:

		template<class T>
		void helpTypes( const T &t ) {
			BSONEmitter bbuild;
			bbuild << "a" << t;
			BSONObj bobj = BSONObjBuilder().append("a", t).obj();
			TS_ASSERT_EQUALS( bobj, bbuild.obj() );
		}

		void testDouble() {
			BSONEmitter bbuild;
			bbuild << "a" << 1.0;
			BSONObj bobj = BSONObjBuilder().append("a", 1.0).obj();
			TS_ASSERT_EQUALS( bobj, bbuild.obj() );
		}

		void testTypes() {
			long long v = 1;
			helpTypes( v );
			bool b = true;
			helpTypes( b );
			int i = 2;
			helpTypes( i );
			double d = 2.1;
			helpTypes( d );
			std::string s = "Hello world!";
			helpTypes( s );
		}


		void testTwoDouble() {
			BSONEmitter bbuild;
			bbuild << "a" << 1.0 << "b" << 2.1;
			BSONObj bobj = BSONObjBuilder().append("a", 1.0).append("b", 2.1).obj();
			TS_ASSERT_EQUALS( bobj, bbuild.obj() );
		}

		void testMemoryManagement() {
			BSONEmitter bbuild;
			bbuild << "a" << 1.0 << "b" << 2.1;
			auto bobj = bbuild.obj();
			std::cout << bobj << std::endl; // This will crash if something went wrong
			std::cout << bobj << std::endl; // This will crash if something went wrong
		}



		void testVector() {
			std::vector<double> v = { 1.1, -2.1 };
			BSONEmitter bbuild;
			bbuild << "a" << v;
			BSONObj bobj = BSONObjBuilder().append("a", v).obj();
			TS_ASSERT_EQUALS( bobj, bbuild.obj() );
		}

		/*void xtestMap() {
			std::map<std::string, double> mymap = {{"a", 1.0}};
			BSONObjBuilder bbuild;
			bbuild << mymap;
			BSONObj bobj = BSONObjBuilder().append("a", 1.0).obj();
			TS_ASSERT_EQUALS( bobj, bbuild.obj() );
		}

		void xtestMapAsValue() {
			std::map<std::string, double> mymap = {{"a", 1.0}};
			BSONObjBuilder bbuild;
			bbuild << "map" << mymap;
			BSONObj bobj = BSONObjBuilder().append( "map",
				BSONObjBuilder().append("a", 1.0).obj() ).obj();
			TS_ASSERT_EQUALS( bobj, bbuild.obj() );
		}*/

		void testVectorAsValue() {
			std::vector<double> vd = {-1.1, 1.0};
			helpTypes( vd );
			std::vector<long long> vl = {1, 2};
			helpTypes( vl );

			std::vector<bool> vb = { true, true };
			helpTypes( vb );
			std::vector<int> vi = {1, 2};
			helpTypes( vi );
			std::vector<std::string> vs = { "Hello world!", "Bla" };
			helpTypes( vs );
		}

		void testSetAsValue() {
			std::set<std::string> vs = { "Hello world!", "Bla" };
			helpTypes( vs );
		}

		void testListAsValue() {
			std::list<std::string> vs = { "Hello world!", "Bla" };
			helpTypes( vs );
		}


		void testTestClassAsValue() {
			test t(-1.1, 1.0);
			BSONEmitter bbuild;
			bbuild << "test" << t;
			auto obj = bbuild.obj();
			BSONObj bobj = BSONObjBuilder().append( "test", 
					BSONObjBuilder().append(
						"a", t.a).append( "b", t.b ).obj()).obj();
			TS_ASSERT_EQUALS( bobj, obj );
		}

		void testTestClassAsVector() {
			std::vector<test> vt = { test(-1.1, 1.0), test( -2.0, -1.1 ) };
			BSONEmitter bbuild;
			bbuild << "test" << vt;
			auto obj = bbuild.obj();
			BSONObj bobj = BSONObjBuilder().append( "test", 
					BSONArrayBuilder()
					.append( 
						BSONObjBuilder().append( "a", vt[0].a).append( "b", vt[0].b ).obj())
					.append( 
						BSONObjBuilder().append( "a", vt[1].a).append( "b", vt[1].b ).obj())
					.arr()).obj();
			TS_ASSERT_EQUALS( bobj, obj );
		}


};
