#include "catch.hpp"

#include "coek/expr/ast_term.hpp"
#include "coek/coek.hpp"

TEST_CASE( "simple_finite", "[smoke]" ) {

  SECTION( "setof_vector_int" ) {
      std::vector<int> v = {1,5,3,7};

      WHEN( "dim" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.size() == 4 );
      }

      WHEN( "finite" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.empty() == false );
      }

      WHEN( "value" ) {
        auto s = coek::SetOf( v );
        for (int i=0; i<4; i++) {
            int ans;
            s.value(i, ans);
            REQUIRE( ans == v[i] );
            }
      }

      WHEN( "contains" ) {
        auto s = coek::SetOf( v );
        for (int i=0; i<4; i++) {
            REQUIRE( s.contains(v[i]) == true );
            }
        }

      WHEN( "begin" ) {
        auto s = coek::SetOf( v );
        auto it = s.begin();
        REQUIRE( (it == s.end()) == false );
        }

      WHEN( "iterate" ) {
        auto s = coek::SetOf( v );
        int i=0;
        for (auto it=s.begin(); it != s.end(); ++it) {
            //REQUIRE( v[i] == std::get<int>((*it)[0]) );
            REQUIRE( v[i] == (*it)[0] );
            i++;
            }
        }
  }

#if 0
  SECTION( "setof_vector_double" ) {
      std::vector<double> v = {1.0,5.0,3.0,7.0};

      WHEN( "dim" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.size() == 4 );
      }

      WHEN( "finite" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.empty() == false );
      }

      WHEN( "value" ) {
        auto s = coek::SetOf( v );
        for (int i=0; i<4; i++) {
            double ans;
            s.value(i, ans);
            REQUIRE( ans == v[i] );
            }
      }

      WHEN( "contains" ) {
        auto s = coek::SetOf( v );
        for (int i=0; i<4; i++) {
            REQUIRE( s.contains(v[i]) == true );
            }
        }

      WHEN( "begin" ) {
        auto s = coek::SetOf( v );
        auto it = s.begin();
        REQUIRE( (it == s.end()) == false );
        }

      WHEN( "iterate" ) {
        auto s = coek::SetOf( v );
        int i=0;
        for (auto it=s.begin(); it != s.end(); ++it) {
            REQUIRE( v[i] == std::get<double>((*it)[0]) );
            i++;
            }
        }
  }

  SECTION( "setof_vector_string" ) {
      std::vector<std::string> v = {"a","c", "b", "d"};

      WHEN( "dim" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.size() == 4 );
      }

      WHEN( "finite" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.empty() == false );
      }

      WHEN( "value" ) {
        auto s = coek::SetOf( v );
        for (int i=0; i<4; i++) {
            std::string ans;
            s.value(i, ans);
            REQUIRE( ans == v[i] );
            }
      }

      WHEN( "contains" ) {
        auto s = coek::SetOf( v );
        for (int i=0; i<4; i++) {
            REQUIRE( s.contains(v[i]) == true );
            }
        }

      WHEN( "begin" ) {
        auto s = coek::SetOf( v );
        auto it = s.begin();
        REQUIRE( (it == s.end()) == false );
        }

      WHEN( "iterate" ) {
        auto s = coek::SetOf( v );
        int i=0;
        for (auto it=s.begin(); it != s.end(); ++it) {
            REQUIRE( v[i] == std::get<std::string>((*it)[0]) );
            i++;
            }
        }
  }
#endif

  SECTION( "setof_set_int" ) {
      std::set<int> v = {1,500,3,7};

      WHEN( "dim" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.size() == 4 );
      }

      WHEN( "finite" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.empty() == false );
      }

      WHEN( "contains" ) {
        auto s = coek::SetOf( v );
        for (auto it=v.begin(); it != v.end(); ++it)
            REQUIRE( s.contains(*it) == true );
        }
  }

#if 0
  SECTION( "setof_set_double" ) {
      std::set<double> v = {1.0,500.0,3.0,7.0};

      WHEN( "dim" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.size() == 4 );
      }

      WHEN( "finite" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.empty() == false );
      }

      WHEN( "contains" ) {
        auto s = coek::SetOf( v );
        for (auto it=v.begin(); it != v.end(); ++it)
            REQUIRE( s.contains(*it) == true );
        }
  }

  SECTION( "setof_set_string" ) {
      std::set<std::string> v = {"a","c","b","d"};

      WHEN( "dim" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.size() == 4 );
      }

      WHEN( "finite" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        auto s = coek::SetOf( v );
        REQUIRE( s.empty() == false );
      }

      WHEN( "contains" ) {
        auto s = coek::SetOf( v );
        for (auto it=v.begin(); it != v.end(); ++it)
            REQUIRE( s.contains(*it) == true );
        }
  }
#endif

  SECTION( "rangeset_int" ) {
      auto s = coek::RangeSet(0,10,2);

      WHEN( "dim" ) {
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        REQUIRE( s.size() == 6 );
        s.initialize();
        REQUIRE( s.size() == 6 );
      }

      WHEN( "finite" ) {
        REQUIRE( s.finite() == true );
        s.initialize();
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        REQUIRE( s.countable() == true );
        s.countable();
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        REQUIRE( s.empty() == false );
        s.initialize();
        REQUIRE( s.empty() == false );
      }

      WHEN( "value" ) {
        for (int i=0; i<6; i++) {
            int ans;
            s.value(i, ans);
            REQUIRE( ans == i*2 );
            }
      }

      WHEN( "contains" ) {
        for (int i=0; i<6; i++) {
            REQUIRE( s.contains(2*i) == true );
            }
        }

      WHEN( "begin" ) {
        auto it = s.begin();
        REQUIRE( (it == s.end()) == false );
        }

      WHEN( "iterate" ) {
        int i=0;
        for (auto it=s.begin(); it != s.end(); ++it) {
            //REQUIRE( 2*i == std::get<int>((*it)[0]) );
            REQUIRE( 2*i == (*it)[0] );
            i++;
            }
        }
  }

#if 0
  SECTION( "rangeset_double" ) {
      auto s = coek::RangeSet(0.0,5.0,0.5);

      WHEN( "dim" ) {
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        REQUIRE( s.size() == 11 );
        s.initialize();
        REQUIRE( s.size() == 11 );
      }

      WHEN( "finite" ) {
        REQUIRE( s.finite() == true );
        s.initialize();
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        REQUIRE( s.countable() == true );
        s.countable();
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        REQUIRE( s.empty() == false );
        s.initialize();
        REQUIRE( s.empty() == false );
      }

      WHEN( "value" ) {
        for (int i=0; i<11; i++) {
            double ans;
            s.value(i, ans);
            REQUIRE( ans == i*0.5 );
            }
      }

      WHEN( "contains" ) {
        for (int i=0; i<6; i++) {
            REQUIRE( s.contains(0.5*i) == true );
            }
      }

      WHEN( "begin" ) {
        auto it = s.begin();
        REQUIRE( (it == s.end()) == false );
        }

      WHEN( "iterate" ) {
        int i=0;
        for (auto it=s.begin(); it != s.end(); ++it) {
            REQUIRE( i*0.5 == std::get<double>((*it)[0]) );
            i++;
            }
        }
  }
#endif

  SECTION( "setof_list_int" ) {
      std::vector<int> v = {1,5,3,7};

      WHEN( "dim" ) {
        auto s = coek::SetOf( {1,5,3,7} );
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( {1,5,3,7} );
        REQUIRE( s.size() == 4 );
      }

      WHEN( "finite" ) {
        auto s = coek::SetOf( {1,5,3,7} );
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        auto s = coek::SetOf( {1,5,3,7} );
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        auto s = coek::SetOf( {1,5,3,7} );
        REQUIRE( s.empty() == false );
      }

      WHEN( "value" ) {
        auto s = coek::SetOf( {1,5,3,7} );
        for (int i=0; i<4; i++) {
            int ans;
            s.value(i, ans);
            REQUIRE( ans == v[i] );
            }
      }

      WHEN( "contains" ) {
        auto s = coek::SetOf( {1,5,3,7} );
        for (int i=0; i<4; i++) {
            REQUIRE( s.contains(v[i]) == true );
            }
        }

      WHEN( "begin" ) {
        auto s = coek::SetOf( {1,5,3,7} );
        auto it = s.begin();
        REQUIRE( (it == s.end()) == false );
        }

      WHEN( "iterate" ) {
        auto s = coek::SetOf( {1,5,3,7} );
        int i=0;
        for (auto it=s.begin(); it != s.end(); ++it) {
            //REQUIRE( v[i] == std::get<int>((*it)[0]) );
            REQUIRE( v[i] == (*it)[0] );
            i++;
            }
        }
  }

#if 0
  SECTION( "setof_list_double" ) {
      std::vector<double> v = {1.0,5.1,3.4,7.3};

      WHEN( "dim" ) {
        auto s = coek::SetOf( {1.0,5.1,3.4,7.3} );
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( {1.0,5.1,3.4,7.3} );
        REQUIRE( s.size() == 4 );
      }

      WHEN( "finite" ) {
        auto s = coek::SetOf( {1.0,5.1,3.4,7.3} );
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        auto s = coek::SetOf( {1.0,5.1,3.4,7.3} );
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        auto s = coek::SetOf( {1.0,5.1,3.4,7.3} );
        REQUIRE( s.empty() == false );
      }

      WHEN( "value" ) {
        auto s = coek::SetOf( {1.0,5.1,3.4,7.3} );
        for (int i=0; i<4; i++) {
            double ans;
            s.value(i, ans);
            REQUIRE( ans == v[i] );
            }
      }

      WHEN( "contains" ) {
        auto s = coek::SetOf( {1.0,5.1,3.4,7.3} );
        for (int i=0; i<4; i++) {
            REQUIRE( s.contains(v[i]) == true );
            }
        }

      WHEN( "begin" ) {
        auto s = coek::SetOf( {1.0,5.1,3.4,7.3} );
        auto it = s.begin();
        REQUIRE( (it == s.end()) == false );
        }

      WHEN( "iterate" ) {
        auto s = coek::SetOf( {1.0,5.1,3.4,7.3} );
        int i=0;
        for (auto it=s.begin(); it != s.end(); ++it) {
            REQUIRE( v[i] == std::get<double>((*it)[0]) );
            i++;
            }
        }
  }

  SECTION( "setof_list_string" ) {
      std::vector<std::string> v = {"a","c","b","d"};

      WHEN( "dim" ) {
        auto s = coek::SetOf( {"a","c","b","d"} );
        REQUIRE( s.dim() == 1 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( {"a","c","b","d"} );
        REQUIRE( s.size() == 4 );
      }

      WHEN( "finite" ) {
        auto s = coek::SetOf( {"a","c","b","d"} );
        REQUIRE( s.finite() == true );
      }

      WHEN( "countable" ) {
        auto s = coek::SetOf( {"a","c","b","d"} );
        REQUIRE( s.countable() == true );
      }

      WHEN( "empty" ) {
        auto s = coek::SetOf( {"a","c","b","d"} );
        REQUIRE( s.empty() == false );
      }

      WHEN( "value" ) {
        auto s = coek::SetOf( {"a","c","b","d"} );
        for (int i=0; i<4; i++) {
            std::string ans;
            s.value(i, ans);
            REQUIRE( ans == v[i] );
            }
      }

      WHEN( "contains" ) {
        auto s = coek::SetOf( {"a","c","b","d"} );
        for (int i=0; i<4; i++) {
            REQUIRE( s.contains(v[i]) == true );
            }
        }

      WHEN( "begin" ) {
        auto s = coek::SetOf( {"a","c","b","d"} );
        auto it = s.begin();
        REQUIRE( (it == s.end()) == false );
        }

      WHEN( "iterate" ) {
        auto s = coek::SetOf( {"a","c","b","d"} );
        int i=0;
        for (auto it=s.begin(); it != s.end(); ++it) {
            REQUIRE( v[i] == std::get<std::string>((*it)[0]) );
            i++;
            }
        }
  }
#endif

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "finite_operations_int", "[smoke]" ) {

    SECTION( "intersection" ) {
        auto s1 = coek::RangeSet(0,6,2);
        auto s2 = coek::SetOf({4,5,6,7,8,9,10});
        auto s = s1.set_intersection(s2);
        std::vector<int> v = {4, 6};

        WHEN( "dim" ) {
            REQUIRE( s.dim() == 1 );
        }

        WHEN( "size" ) {
            auto S = s.initialize();
            REQUIRE( S.size() == 2 );
        }

        WHEN( "finite" ) {
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "empty" ) {
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "value" ) {
            auto S = s.initialize();
            for (int i=0; i<2; i++) {
                int ans;
                S.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }

        WHEN( "contains" ) {
            auto S = s.initialize();
            for (int i=0; i<2; i++) {
                REQUIRE( S.contains(v[i]) == true );
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }
    }

    SECTION( "intersection_AND" ) {
        auto s1 = coek::RangeSet(0,6,2);
        auto s2 = coek::SetOf({4,5,6,7,8,9,10});
        auto s = s1 & s2;
        std::vector<int> v = {4, 6};

        WHEN( "dim" ) {
            REQUIRE( s.dim() == 1 );
        }

        WHEN( "size" ) {
            auto S = s.initialize();
            REQUIRE( S.size() == 2 );
        }

        WHEN( "finite" ) {
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "empty" ) {
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "value" ) {
            auto S = s.initialize();
            for (int i=0; i<2; i++) {
                int ans;
                S.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }

        WHEN( "contains" ) {
            auto S = s.initialize();
            for (int i=0; i<2; i++) {
                REQUIRE( S.contains(v[i]) == true );
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }
    }

    SECTION( "union" ) {
        auto s1 = coek::RangeSet(0,6,2);
        auto s2 = coek::RangeSet(4,10);
        auto s = s1.set_union(s2);
        std::vector<int> v = {0, 2, 4, 6, 5, 7, 8, 9, 10};

        WHEN( "dim" ) {
            REQUIRE( s.dim() == 1 );
        }

        WHEN( "size" ) {
            auto S = s.initialize();
            REQUIRE( S.size() == 9 );
        }

        WHEN( "finite" ) {
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "empty" ) {
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "value" ) {
            auto S = s.initialize();
            for (int i=0; i<9; i++) {
                int ans;
                S.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }

        WHEN( "contains" ) {
            auto S = s.initialize();
            for (int i=0; i<9; i++) {
                REQUIRE( S.contains(v[i]) == true );
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }
    }

    SECTION( "union_PLUS" ) {
        auto s1 = coek::RangeSet(0,6,2);
        auto s2 = coek::RangeSet(4,10);
        auto s = s1 + s2;
        std::vector<int> v = {0, 2, 4, 6, 5, 7, 8, 9, 10};

        WHEN( "dim" ) {
            REQUIRE( s.dim() == 1 );
        }

        WHEN( "size" ) {
            auto S = s.initialize();
            REQUIRE( S.size() == 9 );
        }

        WHEN( "finite" ) {
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "empty" ) {
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "value" ) {
            auto S = s.initialize();
            for (int i=0; i<9; i++) {
                int ans;
                S.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }

        WHEN( "contains" ) {
            auto S = s.initialize();
            for (int i=0; i<9; i++) {
                REQUIRE( S.contains(v[i]) == true );
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }
    }

    SECTION( "union_OR" ) {
        auto s1 = coek::RangeSet(0,6,2);
        auto s2 = coek::RangeSet(4,10);
        auto s = s1 | s2;
        std::vector<int> v = {0, 2, 4, 6, 5, 7, 8, 9, 10};

        WHEN( "dim" ) {
            REQUIRE( s.dim() == 1 );
        }

        WHEN( "size" ) {
            auto S = s.initialize();
            REQUIRE( S.size() == 9 );
        }

        WHEN( "finite" ) {
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "empty" ) {
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "value" ) {
            auto S = s.initialize();
            for (int i=0; i<9; i++) {
                int ans;
                S.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }

        WHEN( "contains" ) {
            auto S = s.initialize();
            for (int i=0; i<9; i++) {
                REQUIRE( S.contains(v[i]) == true );
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }
    }

    SECTION( "difference" ) {
        auto s1 = coek::RangeSet(0,6,2);
        auto s2 = coek::RangeSet(4,10);
        auto s = s1.set_difference(s2);
        std::vector<int> v = {0, 2};

        WHEN( "dim" ) {
            REQUIRE( s.dim() == 1 );
        }

        WHEN( "size" ) {
            auto S = s.initialize();
            REQUIRE( S.size() == 2 );
        }

        WHEN( "finite" ) {
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "empty" ) {
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "value" ) {
            auto S = s.initialize();
            for (int i=0; i<2; i++) {
                int ans;
                S.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }

        WHEN( "contains" ) {
            auto S = s.initialize();
            for (int i=0; i<2; i++) {
                REQUIRE( S.contains(v[i]) == true );
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }
    }

    SECTION( "difference_MINUS" ) {
        auto s1 = coek::RangeSet(0,6,2);
        auto s2 = coek::RangeSet(4,10);
        auto s = s1 - s2;
        std::vector<int> v = {0, 2};

        WHEN( "dim" ) {
            REQUIRE( s.dim() == 1 );
        }

        WHEN( "size" ) {
            auto S = s.initialize();
            REQUIRE( S.size() == 2 );
        }

        WHEN( "finite" ) {
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "empty" ) {
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "value" ) {
            auto S = s.initialize();
            for (int i=0; i<2; i++) {
                int ans;
                S.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }

        WHEN( "contains" ) {
            auto S = s.initialize();
            for (int i=0; i<2; i++) {
                REQUIRE( S.contains(v[i]) == true );
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }
    }

    SECTION( "symmetric_difference" ) {
        auto s1 = coek::RangeSet(0,6,2);
        auto s2 = coek::RangeSet(4,10);
        auto s = s1.set_symmetric_difference(s2);
        std::vector<int> v = {0, 2, 5, 7, 8, 9, 10};

        WHEN( "dim" ) {
            REQUIRE( s.dim() == 1 );
        }

        WHEN( "size" ) {
            auto S = s.initialize();
            REQUIRE( S.size() == 7 );
        }

        WHEN( "finite" ) {
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "empty" ) {
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "value" ) {
            auto S = s.initialize();
            for (int i=0; i<7; i++) {
                int ans;
                S.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }

        WHEN( "contains" ) {
            auto S = s.initialize();
            for (int i=0; i<7; i++) {
                REQUIRE( S.contains(v[i]) == true );
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }
    }

    SECTION( "symmetric_difference_XOR" ) {
        auto s1 = coek::RangeSet(0,6,2);
        auto s2 = coek::RangeSet(4,10);
        auto s = s1 ^ s2;
        std::vector<int> v = {0, 2, 5, 7, 8, 9, 10};

        WHEN( "dim" ) {
            REQUIRE( s.dim() == 1 );
        }

        WHEN( "size" ) {
            auto S = s.initialize();
            REQUIRE( S.size() == 7 );
        }

        WHEN( "finite" ) {
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "empty" ) {
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "value" ) {
            auto S = s.initialize();
            for (int i=0; i<7; i++) {
                int ans;
                S.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }

        WHEN( "contains" ) {
            auto S = s.initialize();
            for (int i=0; i<7; i++) {
                REQUIRE( S.contains(v[i]) == true );
                }
        }

        WHEN( "begin" ) {
            auto S = s.initialize();
            auto it = S.begin();
            REQUIRE( (it == S.end()) == false );
        }

        WHEN( "iterate" ) {
            auto S = s.initialize();
            int i=0;
            for (auto it=S.begin(); it != S.end(); ++it) {
                //REQUIRE( v[i] == std::get<int>((*it)[0]) );
                REQUIRE( v[i] == (*it)[0] );
                i++;
                }
        }
    }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "logical_operations_int", "[smoke]" ) {
    auto s1 = coek::SetOf({1,2});
    auto s2 = coek::SetOf({4,5,6,7,8,9,10});
    auto s3 = coek::SetOf({1,2,4,5,6,7,8,9,10});

    SECTION( "is_disjoint" ) {
        REQUIRE( s1.is_disjoint(s2) == true );
        REQUIRE( s1.is_disjoint(s3) == false );
        REQUIRE( s2.is_disjoint(s1) == true );
        REQUIRE( s2.is_disjoint(s3) == false );
        REQUIRE( s3.is_disjoint(s1) == false );
        REQUIRE( s3.is_disjoint(s2) == false );
    }

    SECTION( "is_subset" ) {
        REQUIRE( s1.is_subset(s2) == false );
        REQUIRE( s1.is_subset(s3) == true );
        REQUIRE( s2.is_subset(s1) == false );
        REQUIRE( s2.is_subset(s3) == true );
        REQUIRE( s3.is_subset(s1) == false );
        REQUIRE( s3.is_subset(s2) == false );
    }

    SECTION( "is_superset" ) {
        REQUIRE( s1.is_superset(s2) == false );
        REQUIRE( s1.is_superset(s3) == false );
        REQUIRE( s2.is_superset(s1) == false );
        REQUIRE( s2.is_superset(s3) == false );
        REQUIRE( s3.is_superset(s1) == true );
        REQUIRE( s3.is_superset(s2) == true );
    }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "product_finite", "[smoke]" ) {

    SECTION( "int_int" ) {
        auto s1 = coek::SetOf({1,2,3});
        auto s2 = coek::SetOf({4,5,6,7});

        WHEN( "dim" ) {
            auto s = s1*s2;
            REQUIRE( s1.dim() == 1 );
            REQUIRE( s2.dim() == 1 );
            REQUIRE( s.dim() == 2 );
        }

        WHEN( "size" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            REQUIRE( S.size() == 12 );
        }

        WHEN( "finite" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "countable" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            REQUIRE( S.countable() == true );
        }

        WHEN( "empty" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

#if 0
        WHEN( "value" ) {
            auto s = s1*s2;
            for (int i=0; i<4; i++) {
                int ans;
                s.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }
#endif

        WHEN( "contains" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            std::vector<coek::set_types> v(2);
            for (size_t i=0; i<s1.size(); i++) {
                int arg1;
                s1.value(i, arg1);
                v[0] = arg1;
                for (size_t j=0; j<s2.size(); j++) {
                    int arg2;
                    s2.value(i, arg2);
                    v[1] = arg2;
                    REQUIRE( S.contains(v) == true );
                }
            }
        }
    }

#if 0
    SECTION( "int_string" ) {
        auto s1 = coek::SetOf({1,2,3});
        auto s2 = coek::SetOf({"aa", "cc", "bb", "dd"});

        WHEN( "dim" ) {
            auto s = s1*s2;
            REQUIRE( s1.dim() == 1 );
            REQUIRE( s2.dim() == 1 );
            REQUIRE( s.dim() == 2 );
        }

        WHEN( "size" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            REQUIRE( S.size() == 12 );
        }

        WHEN( "finite" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "countable" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            REQUIRE( S.countable() == true );
        }

        WHEN( "empty" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

#if 0
        WHEN( "value" ) {
            auto s = s1*s2;
            for (int i=0; i<4; i++) {
                int ans;
                s.value(i, ans);
                REQUIRE( ans == v[i] );
                }
        }
#endif

        WHEN( "contains" ) {
            auto s = s1*s2;
            auto S = s.initialize();
            std::vector<coek::set_types> v(2);
            for (size_t i=0; i<s1.size(); i++) {
                int arg1;
                s1.value(i, arg1);
                v[0] = arg1;
                for (size_t j=0; j<s2.size(); j++) {
                    std::string arg2;
                    s2.value(i, arg2);
                    v[1] = arg2;
                    REQUIRE( S.contains(v) == true );
                }
            }
        }
    }

    SECTION( "int_string_string" ) {
        auto s1 = coek::SetOf({1,2,3});
        auto s2 = coek::SetOf({"aa", "cc", "bb", "dd"});

        WHEN( "dim" ) {
            auto s3 = s1*s2;
            auto s = s3*s2;
            REQUIRE( s.dim() == 3 );
        }

        WHEN( "size" ) {
            auto s3 = s1*s2;
            auto s = s3*s2;
            auto S = s.initialize();
            REQUIRE( S.size() == 48 );
        }

        WHEN( "finite" ) {
            auto s3 = s1*s2;
            auto s = s3*s2;
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "countable" ) {
            auto s3 = s1*s2;
            auto s = s3*s2;
            auto S = s.initialize();
            REQUIRE( S.countable() == true );
        }

        WHEN( "empty" ) {
            auto s3 = s1*s2;
            auto s = s3*s2;
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }
    }

    SECTION( "int_string_string" ) {
        auto s1 = coek::SetOf({1,2,3});
        auto s2 = coek::SetOf({"aa", "cc", "bb", "dd"});

        WHEN( "dim" ) {
            auto s3 = s1*s2;
            auto s = s2*s3;
            REQUIRE( s.dim() == 3 );
        }

        WHEN( "size" ) {
            auto s3 = s1*s2;
            auto s = s2*s3;
            auto S = s.initialize();
            REQUIRE( S.size() == 48 );
        }

        WHEN( "finite" ) {
            auto s3 = s1*s2;
            auto s = s2*s3;
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "countable" ) {
            auto s3 = s1*s2;
            auto s = s2*s3;
            auto S = s.initialize();
            REQUIRE( S.countable() == true );
        }

        WHEN( "empty" ) {
            auto s3 = s1*s2;
            auto s = s2*s3;
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }
    }

    SECTION( "int_string_int_string" ) {
        auto s1 = coek::SetOf({1,2,3});
        auto s2 = coek::SetOf({"aa", "cc", "bb", "dd"});

        WHEN( "dim" ) {
            auto s3 = s1*s2;
            auto s = s3*s3;
            REQUIRE( s.dim() == 4 );
        }

        WHEN( "size" ) {
            auto s3 = s1*s2;
            auto s = s3*s3;
            auto S = s.initialize();
            REQUIRE( S.size() == 144 );
        }

        WHEN( "finite" ) {
            auto s3 = s1*s2;
            auto s = s3*s3;
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "countable" ) {
            auto s3 = s1*s2;
            auto s = s3*s3;
            auto S = s.initialize();
            REQUIRE( S.countable() == true );
        }

        WHEN( "empty" ) {
            auto s3 = s1*s2;
            auto s = s3*s3;
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }
    }
#endif

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "product_finite_operations", "[smoke]" ) {

    SECTION( "intersection" ) {
        auto s1 = coek::SetOf({1,2,3});
        auto s2 = coek::SetOf({4,5,6,7});
        auto s3 = coek::SetOf({6,7,8,9});

        WHEN( "dim" ) {
            auto s = s1*s2 & s1*s3;
            REQUIRE( s.dim() == 2 );
        }

        WHEN( "size" ) {
            auto s = s1*s2 & s1*s3;
            auto S = s.initialize();
            REQUIRE( S.size() == 6 );
        }

        WHEN( "finite" ) {
            auto s = s1*s2 & s1*s3;
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "countable" ) {
            auto s = s1*s2 & s1*s3;
            auto S = s.initialize();
            REQUIRE( S.countable() == true );
        }

        WHEN( "empty" ) {
            auto s = s1*s2 & s1*s3;
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "contains" ) {
            auto s = s1*s2 & s1*s3;
            auto S = s.initialize();
            std::vector<int> RHS = {6,7};
            std::vector<coek::set_types> v(2);
            for (size_t i=0; i<s1.size(); i++) {
                int arg1;
                s1.value(i, arg1);
                v[0] = arg1;
                for (size_t j=0; j<2; j++) {
                    v[1] = RHS[j];
                    REQUIRE( S.contains(v) == true );
                }
            }
        }
    }

    SECTION( "union" ) {
        auto s1 = coek::SetOf({1,2,3});
        auto s2 = coek::SetOf({4,5,6,7});
        auto s3 = coek::SetOf({6,7,8,9});

        WHEN( "dim" ) {
            auto s = s1*s2 + s1*s3;
            REQUIRE( s.dim() == 2 );
        }

        WHEN( "size" ) {
            auto s = s1*s2 + s1*s3;
            auto S = s.initialize();
            REQUIRE( S.size() == 18 );
        }

        WHEN( "finite" ) {
            auto s = s1*s2 + s1*s3;
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "countable" ) {
            auto s = s1*s2 + s1*s3;
            auto S = s.initialize();
            REQUIRE( S.countable() == true );
        }

        WHEN( "empty" ) {
            auto s = s1*s2 + s1*s3;
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "contains" ) {
            auto s = s1*s2 + s1*s3;
            auto S = s.initialize();
            std::vector<int> RHS = {4,5,6,7,8,9};
            std::vector<coek::set_types> v(2);
            for (size_t i=0; i<s1.size(); i++) {
                int arg1;
                s1.value(i, arg1);
                v[0] = arg1;
                for (size_t j=0; j<2; j++) {
                    v[1] = RHS[j];
                    REQUIRE( S.contains(v) == true );
                }
            }
        }
    }

    SECTION( "difference" ) {
        auto s1 = coek::SetOf({1,2,3});
        auto s2 = coek::SetOf({4,5,6,7});
        auto s3 = coek::SetOf({6,7,8,9});

        WHEN( "dim" ) {
            auto s = s1*s2 - s1*s3;
            REQUIRE( s.dim() == 2 );
        }

        WHEN( "size" ) {
            auto s = s1*s2 - s1*s3;
            auto S = s.initialize();
            REQUIRE( S.size() == 6 );
        }

        WHEN( "finite" ) {
            auto s = s1*s2 - s1*s3;
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "countable" ) {
            auto s = s1*s2 - s1*s3;
            auto S = s.initialize();
            REQUIRE( S.countable() == true );
        }

        WHEN( "empty" ) {
            auto s = s1*s2 - s1*s3;
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "contains" ) {
            auto s = s1*s2 - s1*s3;
            auto S = s.initialize();
            std::vector<int> RHS = {4,5};
            std::vector<coek::set_types> v(2);
            for (size_t i=0; i<s1.size(); i++) {
                int arg1;
                s1.value(i, arg1);
                v[0] = arg1;
                for (size_t j=0; j<2; j++) {
                    v[1] = RHS[j];
                    REQUIRE( S.contains(v) == true );
                }
            }
        }
    }

    SECTION( "symmetric_difference" ) {
        auto s1 = coek::SetOf({1,2,3});
        auto s2 = coek::SetOf({4,5,6,7});
        auto s3 = coek::SetOf({6,7,8,9});

        WHEN( "dim" ) {
            auto s = s1*s2 ^ s1*s3;
            REQUIRE( s.dim() == 2 );
        }

        WHEN( "size" ) {
            auto s = s1*s2 ^ s1*s3;
            auto S = s.initialize();
            REQUIRE( S.size() == 12 );
        }

        WHEN( "finite" ) {
            auto s = s1*s2 ^ s1*s3;
            auto S = s.initialize();
            REQUIRE( S.finite() == true );
        }

        WHEN( "countable" ) {
            auto s = s1*s2 ^ s1*s3;
            auto S = s.initialize();
            REQUIRE( S.countable() == true );
        }

        WHEN( "empty" ) {
            auto s = s1*s2 ^ s1*s3;
            auto S = s.initialize();
            REQUIRE( S.empty() == false );
        }

        WHEN( "contains" ) {
            auto s = s1*s2 ^ s1*s3;
            auto S = s.initialize();
            std::vector<int> RHS = {4,5,8,9};
            std::vector<coek::set_types> v(2);
            for (size_t i=0; i<s1.size(); i++) {
                int arg1;
                s1.value(i, arg1);
                v[0] = arg1;
                for (size_t j=0; j<2; j++) {
                    v[1] = RHS[j];
                    REQUIRE( S.contains(v) == true );
                }
            }
        }
    }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
