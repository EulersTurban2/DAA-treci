#include <iostream>

#include <map>
#include <set>
#include <cmath>
#include <ctime>
#include <queue>
#include <vector>
#include <algorithm>

#define DEBUG 1

const double EPS = 1e-9;
double sweepX = 0;

template<typename T>
struct Point{
    T x, y;
    bool operator<(const Point<T>& p2) const{
        return x < p2.x || (x == p2.x && y < p2.y);    
    }
    bool operator==(const Point<T>& p2) const{
           return x == p2.x && y == p2.y;
    }
};

template<typename T>
struct Segment{
    Point<T> le,re;
    int id;

    Segment(Point<T> p1, Point<T> p2, int _id) : id(_id) {
        if(p2 < p1) 
            std::swap(p1,p2);
        le = p1;
        re = p2;
    }

    double get_y(double x) const {
        if(std::abs(le.x-re.x) < EPS)
            return le.y;
        return le.y + (re.y - le.y) * (x - le.x) / (re.x - le.x);
    }
};

// Komparator za segmente, sortiramo ih vertikalno, u slucaju da se poklapaju onda sta radimo po dodjeljenom id-u!

template<typename T>
struct SweepCmp {
    bool operator()(const Segment<T>* s1, const Segment<T>* s2) const{
        if(s1 == s2)
            return false;
        double y1 = s1->get_y(sweepX);
        double y2 = s2->get_y(sweepX);
        if(std::abs(y1 - y2) > EPS)
            return y1 < y2;
        return s1->id < s2->id;
    }
};

template<typename T>
bool getIntersectionX(const Segment<T>& s1, const Segment<T>& s2, double& outX){
    double s1x1 = s1.le.x, s1y1 = s1.le.y, s1x2 = s1.re.x, s1y2 = s1.re.y;
    double s2x1 = s2.le.x, s2y1 = s2.le.y, s2x2 = s2.re.x, s2y2 = s2.re.y;
    
    double denom = (s1x1 - s1x2) * (s2y1 - s2y2) - (s1y1 - s1y2) * (s2x1 - s2x2);
    if (std::abs(denom) < 1e-12) 
        return false; 

    double px = ((s1x1 * s1y2 - s1y1 * s1x2) * (s2x1 - s2x2) - (s1x1 - s1x2) * (s2x1 * s2y2 - s2y1 * s2x2)) / denom;
    if (px < std::max(s1x1,s2x1) - EPS || px > std::min(s1x2,s2x2) + EPS)
        return false;
    
    outX = px;
    return true;
}

enum EventType { START, PRESJEK, END };

struct Event
{
    double x;
    EventType type;
    Segment<double> *s1, *s2;

    bool operator>(const Event& other) const{
        if(std::abs(x - other.x) > EPS)
            return x > other.x;
        return type > other.type;
    }
};


std::map<int, std::vector<int>> bentleyOttman(std::vector<Segment<double>> segments, const std::vector<int>& targets) {
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> pq;
    std::set<Segment<double>*, SweepCmp<double>> sweep;
    std::set<int> targetSet(targets.begin(),targets.end());
    std::map<int, std::vector<int>> result;

    auto schedule = [&](Segment<double>* s1, Segment<double>* s2) {
        if(!s1 || !s2)
            return;
        double ix;
        if (getIntersectionX(*s1,*s2,ix))
        {
            if (ix > sweepX + EPS)
            {
                pq.push({ix,EventType::PRESJEK, s1, s2});
            }
        }
    };

    for (auto& s: segments)
    {
        pq.push({s.le.x, EventType::START, &s, nullptr});
        pq.push({s.re.x, EventType::END, &s, nullptr});
    }

    while (!pq.empty())
    {
        Event e = pq.top();
        pq.pop();
        sweepX = e.x;

        if (e.type == EventType::START)
        {
            auto it = sweep.insert(e.s1).first;
            auto nxt = std::next(it);
            auto prv = (it == sweep.begin() ? sweep.end() : std::prev(it));

            if (nxt != sweep.end()) schedule(*it, *nxt);
            if (prv != sweep.end()) schedule(*prv, *it);
        }
        else if (e.type == EventType::END) {
            auto it = sweep.find(e.s1);
            if (it != sweep.end()) {
                auto nxt = std::next(it);
                auto prv = (it == sweep.begin() ? sweep.end() : std::prev(it));
                if (prv != sweep.end() && nxt != sweep.end()) 
                    schedule(*prv, *nxt);
                sweep.erase(it);
            }
        }
        else if (e.type == EventType::PRESJEK){
            Segment<double>* A = e.s1;
            Segment<double>* B = e.s2;

            if (targetSet.count(A->id)) 
                result[A->id].push_back(B->id);
            if (targetSet.count(B->id)) 
                result[B->id].push_back(A->id);

            sweep.erase(A);
            sweep.erase(B);

            sweepX += EPS; 
            sweep.insert(A);
            sweep.insert(B);
            sweepX -= EPS;

            auto itA = sweep.find(A), itB = sweep.find(B);
            if (itA != sweep.end()) {
                auto nxt = std::next(itA);
                auto prv = std::prev(itA); 
                if (nxt != sweep.end()) 
                    schedule(A, *nxt);
                if (itA != sweep.begin()) 
                    schedule(*prv, A);
            }
            if (itB != sweep.end()) {
                auto nxt = std::next(itB); 
                auto prv = std::prev(itB);
                if (nxt != sweep.end()) 
                    schedule(B, *nxt);
                if (itB != sweep.begin()) 
                    schedule(*prv, B);
            }
        }
    }
    return result;
}

int main(){

    int num_points;
    std::cin >> num_points;
    

    //std::cout << tmp << std::endl;
    // INPUT
    std::vector<Point<double>> p_set;
    for (int i = 0; i < num_points; i++)
    {
        Point<double> tmp;
        std::cin >> tmp.x >> tmp.y;
        p_set.push_back(tmp);
    }
    Point<double> O;
    std::cin >> O.x >> O.y;

    std::vector<Segment<double>> segments;
    int counter = 0;
    for (int i = 0; i < num_points; i++)
    {
        for (int j = i+1; j < num_points; j++)
        {
            segments.emplace_back(p_set[i],p_set[j],counter);
            counter++;
        }
        
    }
    int tmp = segments.size();
    std::vector<int> positions;
    for (int i = 0; i < num_points; i++)
    {
        segments.emplace_back(O,p_set[i],counter);
        positions.push_back(counter);
        counter++;
    }
    
    auto report = bentleyOttman(segments,positions);

    std::vector<bool> mask(num_points,true);
    for (auto const& [targetId, hits] : report)
    {
        int position_of_points = targetId - tmp;
        mask[position_of_points] = false;
    }
    
    for (int i = 0; i < num_points; i++)
    {
        if(mask[i]) {
            std::cout << p_set[i].x << " " << p_set[i].y << std::endl;
        }
    }
    
    return 0;
}