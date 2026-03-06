package com.djordjemilanovic.minesweeper
package utils

import org.scalatest.funsuite.AnyFunSuite

class DimensionsTest extends AnyFunSuite {
  test("Location is within dimensions") {
    assert(Dimensions(5, 10).contains(Location(4, 9)))
  }

  test("Location has negative coordinates") {
    assert(!Dimensions(5, 10).contains(Location(-1, 0)))
  }

  test("Location has coordinates outside dimensions") {
    assert(!Dimensions(5, 10).contains(Location(5, 0)))
  }

  test("String representation") {
    assert(Dimensions(5, 10).toString == "(5 X 10)")
  }

  test("Area") {
    assert(Dimensions(5, 10).area == 50)
  }

  test("Transposed dimensions") {
    assert(Dimensions(5, 10).transposed == Dimensions(10, 5))
  }
}
