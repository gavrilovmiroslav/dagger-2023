#!/bin/bash
dotnet tool restore
dotnet coffeecup "$(pwd)/tools/CoffeeCup/Config.yaml"