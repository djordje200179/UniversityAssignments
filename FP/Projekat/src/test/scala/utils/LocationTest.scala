package com.djordjemilanovic.minesweeper
package utils

import org.scalatest.funsuite.AnyFunSuite

class LocationTest extends AnyFunSuite {

  test("Neighbours around corner") {
    val dimensions = Dimensions(5, 5)
    val location = Location(0, 0)

    assert(location.neighbors(dimensions).toSet == Set(
      Location(0, 1),
      Location(1, 0),
      Location(1, 1)
    ))
  }

  test("Neighbours around edge") {
    val dimensions = Dimensions(5, 5)
    val location = Location(0, 2)

    val neighbors = location.neighbors(dimensions)

    assert(neighbors.toSet == Set(
      Location(0, 1),
      Location(0, 3),
      Location(1, 1),
      Location(1, 2),
      Location(1, 3)
    ))
  }

  test("Neighbours in the middle") {
    val dimensions = Dimensions(5, 5)
    val location = Location(2, 2)

    val neighbors = location.neighbors(dimensions)

    assert(neighbors.toSet == Set(
      Location(1, 1),
      Location(1, 2),
      Location(1, 3),
      Location(2, 1),
      Location(2, 3),
      Location(3, 1),
      Location(3, 2),
      Location(3, 3)
    ))
  }

  test("String representation") {
    assert(Location(2, 3).toString == "(2, 3)")
  }

  test("Random location within dimensions") {
    val dimensions = Dimensions(5, 10)

    assert(dimensions.contains(Location.random(dimensions)))
  }
  
  test("Origin location") {
    assert(Location.origin == Location(0, 0))
  }
}
