class complex {
private:
  double re;
  double im;

public:
  complex(double re = 0, double im = 0);
  double getX();
  double getY();
  bool equals(const complex &other) const;
};