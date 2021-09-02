#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>

/**
 * Coordinate class.
 */
class Coordinate {
public:
  /**
   * Constructs coordinate object with position {-1, -1}.
   */
  Coordinate() : m_rank(-1), m_file(-1){};

  /**
   * Constructs coordinate with given rank and file.
   *
   * @param rank Vertical position on board.
   * @param file Horizontal position on board.
   */
  Coordinate(int rank, int file) : m_rank(rank), m_file(file){};

  /**
   * Constructs copy of coordinate.
   *
   * @param c Coordinate to copy.
   */
  Coordinate(const Coordinate &c) {
    m_rank = c.m_rank;
    m_file = c.m_file;
  }

  /**
   * Getter for `m_rank`.
   *
   * @return Vertical position of coordinate.
   */
  int get_rank() const { return m_rank; }

  /**
   * Getter for `m_file`.
   *
   * @return Horizontal position of coordinate.
   */
  int get_file() const { return m_file; }

  /**
   * Overload equality operator for coordinate object.
   *
   * @param c1 Coordinate 1.
   * @param c2 Coordinate 2.
   * @return Boolean if ranks and files are both equal.
   */
  friend bool operator==(const Coordinate &c1, const Coordinate &c2);

  /**
   * Overload insertion operator for coordinate object.
   *
   * @param os Stream to output to.
   * @param c Coordinate to output.
   * @return Reference to stream outputted to.
   */
  friend std::ostream &operator<<(std::ostream &os, const Coordinate &c);

private:
  /// Vertical position of coordinate.
  int m_rank;

  /// Horizontal position of coordinate.
  int m_file;
};

#endif