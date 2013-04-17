#!/bin/bash
rsync --password-file="~/.pwd" -avz _site/ $(cat ~/.user)@$(cat ~/.site):~/.
