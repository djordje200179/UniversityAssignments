package com.djordjemilanovic.minesweeper

import minefield.Minefield.Cell.*

package object minefield {
  val sampleMinefield = Minefield(Vector(
    Vector(Empty, Mine, Empty),
    Vector(Mine, Empty, Mine),
    Vector(Empty, Empty, Empty)
  ))
}
