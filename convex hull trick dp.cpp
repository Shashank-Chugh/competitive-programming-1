const int MAXN = 5e4 + 10;
const long long INF = 1e18;

class ConvexHullDynamic {
     typedef long long coef_t, coord_t, val_t;
//Line 'y=a*x+b' represented by 2 coefficients 'a' and 'b'
private:
     struct Line {
          coef_t a, b;
          double xLeft;
          enum Type {line, maxQuery, minQuery} type;
          coord_t val;
          explicit Line(coef_t aa = 0, coef_t bb = 0) : a(aa), b(bb), xLeft(-INFINITY), type(Type::line), val(0) {}
          val_t valueAt(coord_t x) const { return a * x + b; }
          friend bool areParallel(const Line& l1, const Line& l2) { return l1.a == l2.a; }
          friend double intersectX(const Line& l1, const Line& l2) { return areParallel(l1, l2) ? INFINITY : 1.0 * (l2.b - l1.b) / (l1.a - l2.a); }
          bool operator<(const Line& l2) const {
               if (l2.type == line) return this->a > l2.a;
               if (l2.type == maxQuery) return this->xLeft < l2.val;
               if (l2.type == minQuery) return this->xLeft > l2.val;
               assert(0);
               return false;
          }
     };
//private:
public:
     bool            isMax; //whether or not saved envelope is top(search of max value)
     std::set<Line>  hull;  //envelope itself
private:
     bool hasPrev(std::set<Line>::iterator it) { return it != hull.begin(); }
     bool hasNext(std::set<Line>::iterator it) { return it != hull.end() && std::next(it) != hull.end(); }
     bool irrelevant(const Line& l1, const Line& l2, const Line& l3) { return intersectX(l1, l3) <= intersectX(l1, l2); }
     bool irrelevant(std::set<Line>::iterator it) {
          return hasPrev(it) && hasNext(it) && ((isMax && irrelevant(*std::prev(it), *it, *std::next(it)))
                                                || (!isMax && irrelevant(*std::next(it), *it, *std::prev(it))));
     }
     std::set<Line>::iterator updateLeftBorder(std::set<Line>::iterator it) {
          if ((isMax && !hasPrev(it)) || (!isMax && !hasNext(it))) return it;
          double val = intersectX(*it, isMax ? *std::prev(it) : *std::next(it));
          Line buf(*it);
          it = hull.erase(it), buf.xLeft = val, it = hull.insert(it, buf);
          return it;
     }
public:
     explicit ConvexHullDynamic(): isMax(1) {} // true is for max and false is for min
     void addLine(coef_t a, coef_t b) {
          Line l3 = Line(a, b);
          auto it = hull.lower_bound(l3);
          if (it != hull.end() && areParallel(*it, l3)) {
               if ((isMax && it->b < b) || (!isMax && it->b > b)) it = hull.erase(it);
               else return;
          }
          it = hull.insert(it, l3);
          if (irrelevant(it)) { hull.erase(it); return; }
          while (hasPrev(it) && irrelevant(std::prev(it))) hull.erase(std::prev(it));
          while (hasNext(it) && irrelevant(std::next(it))) hull.erase(std::next(it));
          it = updateLeftBorder(it);
          if (hasPrev(it)) updateLeftBorder(std::prev(it));
          if (hasNext(it)) updateLeftBorder(std::next(it));
     }
     val_t getBest(coord_t x) const {
          Line q;
          q.val = x, q.type = isMax ? Line::Type::maxQuery : Line::Type::minQuery;
          auto bestLine = hull.lower_bound(q);
          if (isMax) --bestLine;
          return bestLine->valueAt(x);
     }
} H;
