VERSION      = 6.10
KERNEL_DIR   = /usr/src/linux-$(VERSION)
KERNEL_SUFFIX = -procmem
MAKEFLAGS += -j1
PATCH_FILE = 0001-Add-get_pid_info-system-call.patch

SRC_SYSCALL  = sys_get_pid_info.c
HDR_SYSCALL  = pid_info.h

all: fetch-linux patch-kernel compile install
    
fetch-linux:
	@if [ ! -d "$(KERNEL_DIR)" ]; then \
		git clone --depth=1 --branch v$(VERSION) https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git $(KERNEL_DIR); \
	else \
			echo "Kernel source exists. Resetting to clean state..."; \
			cd $(KERNEL_DIR) && git am --abort 2>/dev/null || true; \
			cd $(KERNEL_DIR) && git reset --hard v$(VERSION); \
			cd $(KERNEL_DIR) && git clean -fdx; \
	fi

patch-kernel:
	@cd $(KERNEL_DIR) && git am $(CURDIR)/$(PATCH_FILE)

compile:
	@cp /boot/config-$(VERSION) $(KERNEL_DIR)/.config
	@make -C $(KERNEL_DIR) olddefconfig;
	@cd $(KERNEL_DIR) && make -j$(shell nproc)

install:
	@cp -f $(KERNEL_DIR)/arch/x86/boot/bzImage /boot/vmlinuz-$(VERSION)$(KERNEL_SUFFIX)
	@cp -f $(KERNEL_DIR)/System.map /boot/System.map-$(VERSION)$(KERNEL_SUFFIX)
	@cp -f $(KERNEL_DIR)/.config /boot/config-$(VERSION)$(KERNEL_SUFFIX)
	@cd $(KERNEL_DIR) && make modules_install
	@grub-mkconfig -o /boot/grub/grub.cfg
	@echo "[SUCCESS]"

patch-glibc:
	@cp user_pid_info.h /usr/include/pid_info.h
	@chmod 644 /usr/include/pid_info.h
	@echo "[SUCCESS] Header installed"
	
.PHONY: all fetch-linux patch-kernel compile install