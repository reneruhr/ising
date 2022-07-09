constexpr int size {10};
constexpr int N = size*size;

struct Site {
     int i_; int j_;
};

enum class Nbs { TOP, BOTTOM, LEFT, RIGHT };
auto nbs = std::vector<Nbs> { Nbs::TOP, Nbs::BOTTOM, Nbs::LEFT, Nbs::RIGHT };
using Flip = bool;

Site operator+(Site s, Nbs b){
     switch(b){
     case(Nbs::TOP): return {s.i_-1,s.j_};
     case(Nbs::BOTTOM): return {s.i_+1,s.j_};
     case(Nbs::LEFT): return {s.i_,s.j_-1};
     case(Nbs::RIGHT): return {s.i_,s.j_+1};
     }
}

int main(){
     float T{2.5f};

     int num_iterations;

     std::random_device r;
     std::mt19937 mt(r());
     auto RandomProb = [&mt](){ static std::uniform_real_distribution<float> dist(0.f, 1.f); return dist(mt); };
     auto RandomInteger = [&mt](){static std::uniform_int_distribution<> dist(0, size-1); return dist(mt); };
     auto RandomBool = [&mt](){static std::uniform_int_distribution<short> dist(0,1); return dist(mt) > 0; };

     auto Sign = [](Flip f){ return f ? 1 : -1; };
     auto MakeFlip = [](int i) { return i>= 0 ? true : false; };
     auto RandomizeFlip = [&RandomBool](Flip& f) { f = RandomBool(); };
     using Row = std::array<Flip, size>;
     auto omega = std::array<Row,size>{};
     auto up_flip= MakeFlip(1); 


     auto Boundary = [](Site s) { return s.i_ == 0 || s.i_ == size-1 || s.j_ == 0 || s.j_ == size-1; };
     auto Omega = [&omega](Site s) { return omega[s.i_][s.j_]; };
     auto FlipSite = [&omega](Site s) { omega[s.i_][s.j_]^=true; };

     auto PeriodicBoundary = [&Omega] (Site s){
	  return  Omega({(s.i_+ size) % size, (s.j_ + size) % size});
     };

     auto NbSign = [&Sign, &PeriodicBoundary](Site s, Nbs nb){
	  //if(Boundary(s)) return Sign(up_flip); /// Fixed + boundary
	  // return Sign(Omega(s+nb));
	  return Sign(PeriodicBoundary(s)); 
     };

     auto RandomSite = [&RandomInteger](){
	  return Site{ RandomInteger(), RandomInteger()};
     };

     auto EnergyDifference= [&Omega, &NbSign](Site s){
	  using enum Nbs;
	  auto signs = std::vector<int>();
	  using std::begin; using std::end;
	  std::transform(begin(nbs), end(nbs), std::back_inserter(signs), [&s, &NbSign](Nbs x) { return NbSign(s,x); });
	  return 2*Omega(s)* std::accumulate(begin(signs),end(signs), 0);
	  //(NbSign(s, TOP)+NbSign(s, BOTTOM)+NbSign(s, LEFT), NbSign(s, RIGHT)); 
     };

     auto ColorSite = [](Site s){};

     auto MeanMagnetization = [&omega, &Sign] (){
	  using std::begin; using std::end;
	  return std::accumulate(
	       begin(omega), end(omega), 0.,
	       [&Sign](int sum, Row& row) { return std::accumulate(
			 begin(row), end(row), sum,
			 [&Sign](int sum, Flip b){ return sum+Sign(b); });
	       })/N;
     };

     auto ColdStart = [&up_flip](Flip& f) { f = up_flip; };

     auto Initialize = [&](auto Law){
	  for(auto& row : omega)
	       for(auto& f : row)
		    Law(f);
     };	

     auto Run = [&](){
	  for(auto k{0}; k<num_iterations*size*size; ++k){
	       auto s = RandomSite();
	       auto delta = EnergyDifference(s);
	       if(delta <= 0 || RandomProb() < std::exp(-delta/T) ) { FlipSite(s); ColorSite(s); }
	  }
     };

     Initialize(RandomizeFlip); 
     Run();
     std::cout << "Done.\n";
}
