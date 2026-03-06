package com.djordjemilanovic.minesweeper
package minefield.editors

import minefield.Minefield
import minefield.editors.Transforming.ReflectionAxis.*
import minefield.editors.Transforming.RotationDirection.*
import utils.Location

object Transforming {
  def rotated(direction: RotationDirection)(minefield: Minefield): Minefield = Minefield(
    Vector.tabulate(minefield.dimensions.width, minefield.dimensions.height) { (x, y) =>
      direction match {
        case Clockwise        => minefield(Location(minefield.dimensions.height - y - 1, x))
        case CounterClockwise => minefield(Location(y, minefield.dimensions.width - x - 1))
      }
    }
  )

  def reflected(axis: ReflectionAxis)(minefield: Minefield): Minefield = axis match {
    case Vertical => Minefield(minefield.mines.map(_.reverse))
    case Horizontal => Minefield(minefield.mines.reverse)
    case MainDiagonal => Minefield(Vector.tabulate(minefield.dimensions.width, minefield.dimensions.height) { (y, x) =>
      minefield(Location(x, y))
    })
    case AntiDiagonal => Minefield(Vector.tabulate(minefield.dimensions.width, minefield.dimensions.height) { (y, x) =>
      minefield(Location(minefield.dimensions.height - x - 1, minefield.dimensions.width - y - 1))
    })
  }

  enum RotationDirection {
    case Clockwise, CounterClockwise
  }

  enum ReflectionAxis {
    case Vertical, Horizontal, MainDiagonal, AntiDiagonal
  }
}