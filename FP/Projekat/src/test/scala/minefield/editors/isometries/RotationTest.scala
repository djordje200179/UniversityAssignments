package com.djordjemilanovic.minesweeper
package minefield.editors.isometries

import minefield.Minefield.Cell.Empty
import minefield.editors.Transforming.RotationDirection.{Clockwise, CounterClockwise}
import minefield.editors.{Isometry, Transforming}
import minefield.sampleMinefield
import utils.{Location, Sector}

import org.scalatest.funsuite.AnyFunSuite

class RotationTest extends AnyFunSuite {
  test("Clockwise rotation without transparency") {
    val sector = Sector(Location(1, 1), Location(2, 2))
    val isometry = Rotation(sector.dimensions, Clockwise, Location(0, 1))

    val result = Isometry(false, true)(isometry)(sector)(sampleMinefield)
    val imageSector = Sector(Location(0, 1), Location(1, 2))

    val expectedMinefield = Transforming.rotated(Clockwise)(sampleMinefield.extracted(sector))
    assert(result.extracted(imageSector) == expectedMinefield)
    assert(result(Location(0, 1)) == Empty)
  }

  test("Clockwise rotation with transparency") {
    val sector = Sector(Location(1, 1), Location(2, 2))
    val isometry = Rotation(sector.dimensions, Clockwise, Location(0, 1))

    val result = Isometry(false, false)(isometry)(sector)(sampleMinefield)
    val imageSector = Sector(Location(0, 1), Location(1, 2))

    assert(result(Location(0, 1)) == sampleMinefield(Location(0, 1)))
  }

  test("Counter-clockwise rotation without transparency") {
    val sector = Sector(Location(1, 0), Location(2, 1))
    val isometry = Rotation(sector.dimensions, CounterClockwise, Location(0, 0))

    val result = Isometry(false, true)(isometry)(sector)(sampleMinefield)
    val imageSector = Sector(Location(0, 0), Location(1, 1))

    val expectedMinefield = Transforming.rotated(CounterClockwise)(sampleMinefield.extracted(sector))
    assert(result.extracted(imageSector) == expectedMinefield)
    assert(result(Location(0, 1)) == Empty)
  }

  test("Central clockwise rotation") {
    val isometry = Rotation(sampleMinefield.dimensions, Clockwise, Location(1, 1))

    val result = Isometry(false, true)(isometry)(Sector(Location(0, 0), Location(2, 2)))(sampleMinefield)

    assert(result == Transforming.rotated(Clockwise)(sampleMinefield))
  }
}
