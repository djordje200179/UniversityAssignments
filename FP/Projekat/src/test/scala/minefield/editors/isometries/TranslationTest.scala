package com.djordjemilanovic.minesweeper
package minefield.editors.isometries

import minefield.editors.Isometry
import minefield.sampleMinefield
import utils.{Location, Sector}

import org.scalatest.funsuite.AnyFunSuite

class TranslationTest extends AnyFunSuite {
  test("Non-expanding translation") {
    val sector = Sector(Location(0, 0), Location(1, 1))
    val isometry = Translation(0, 1)

    val result = Isometry(false, false)(isometry)(sector)(sampleMinefield)
    assert(result.dimensions == sampleMinefield.dimensions)
    assert(result(Location(0, 2)) == sampleMinefield(Location(0, 1)))
  }

  test("Expanding translation") {
    val sector = Sector(Location(0, 0), Location(1, 1))
    val isometry = Translation(0, 2)

    val result = Isometry(true, false)(isometry)(sector)(sampleMinefield)
    assert(result.dimensions.width == sampleMinefield.dimensions.width + 1)
    assert(result(Location(0, 3)) == sampleMinefield(Location(0, 1)))
  }
}
