package com.djordjemilanovic.minesweeper
package minefield.editors.isometries

import minefield.Minefield.Cell.*
import minefield.editors.Isometry
import minefield.{Minefield, sampleMinefield}
import utils.{Location, Sector}

import org.scalatest.funsuite.AnyFunSuite

class CentralSymmetryTest extends AnyFunSuite {
  test("Symmetry with a single mine cell") {
    val minefield = Minefield(Vector(
      Vector(Empty, Empty, Empty, Empty, Empty),
      Vector(Empty, Empty, Empty, Empty, Empty),
      Vector(Empty, Empty, Mine, Empty, Empty),
      Vector(Empty, Empty, Empty, Empty, Empty),
      Vector(Empty, Empty, Empty, Empty, Empty)
    ))
    val sector = Sector(Location(2, 2), Location(3, 3))
    val isometry = CentralSymmetry(sector.dimensions, CentralSymmetry.Direction.UpLeft)

    val result = Isometry(false, false)(isometry)(sector)(minefield)

    val expected = Minefield(Vector(
      Vector(Empty, Empty, Empty, Empty, Empty),
      Vector(Empty, Mine, Empty, Empty, Empty),
      Vector(Empty, Empty, Mine, Empty, Empty),
      Vector(Empty, Empty, Empty, Empty, Empty),
      Vector(Empty, Empty, Empty, Empty, Empty)
    ))
  }

  test("Expanding symmetry") {
    val sector = Sector(Location(0, 1), Location(1, 2))
    val isometry = CentralSymmetry(sector.dimensions, CentralSymmetry.Direction.DownRight)

    val result = Isometry(true, false)(isometry)(sector)(sampleMinefield)

    assert(result.dimensions.width == sampleMinefield.dimensions.width + 2)
    assert(result.dimensions.height == sampleMinefield.dimensions.height + 1)
    assert(result(Location(2, 3)) == sampleMinefield(Location(1, 2)))
    assert(result(Location(3, 4)) == sampleMinefield(Location(0, 1)))
  }
}
