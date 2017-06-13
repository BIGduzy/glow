.PHONY: clean All

All:
	@echo "----------Building project:[ week-1 - Release ]----------"
	@cd "week-1" && "$(MAKE)" -f  "week-1.mk"
clean:
	@echo "----------Cleaning project:[ week-1 - Release ]----------"
	@cd "week-1" && "$(MAKE)" -f  "week-1.mk" clean
