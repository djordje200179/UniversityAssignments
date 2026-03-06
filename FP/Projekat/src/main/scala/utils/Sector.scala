package com.djordjemilanovic.minesweeper
package utils

case class Sector(upperLeft: Location, lowerRight: Location) {
  require(upperLeft.x <= lowerRight.x && upperLeft.y <= lowerRight.y)
  
  def dimensions = Dimensions(
    height = lowerRight.y - upperLeft.y + 1,
    width = lowerRight.x - upperLeft.x + 1
  )
}
