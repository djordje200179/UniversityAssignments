package com.djordjemilanovic.minesweeper
package minefield.editors.isometries

import minefield.Minefield
import minefield.editors.Transforming.ReflectionAxis
import minefield.editors.{Isometry, Transforming}
import utils.{Dimensions, Location}

class Reflection(dimensions: Dimensions, axis: ReflectionAxis, index: Option[Int]) extends Isometry {
  require(
    (axis == ReflectionAxis.MainDiagonal || axis == ReflectionAxis.AntiDiagonal) == index.isEmpty,
    "Offset should not be provided for diagonal reflections"
  )

  val offset: Location = axis match {
    case ReflectionAxis.Vertical => Location(0, 2 * index.get - dimensions.width + 1)
    case ReflectionAxis.Horizontal => Location(2 * index.get - dimensions.height + 1, 0)
    case ReflectionAxis.MainDiagonal | ReflectionAxis.AntiDiagonal => Location(0, 0)
  }

  def apply(minefield: Minefield): Minefield = {
    assert(dimensions == minefield.dimensions)
    Transforming.reflected(axis)(minefield)
  }

  def inverse: Isometry = this // Reflection(axis, offset(dimensions).map(-_))
}
