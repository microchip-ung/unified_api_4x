/*
 *
 * Jaguar2 UIO driver.
 *
 * Copyright (C) 2014 Vitesse Semiconductor Inc.
 * Author: Lars Povlsen (lpovlsen@vitesse.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * VITESSE SEMICONDUCTOR INC SHALL HAVE NO LIABILITY WHATSOEVER OF ANY
 * KIND ARISING OUT OF OR RELATED TO THE PROGRAM OR THE OPEN SOURCE
 * MATERIALS UNDER ANY THEORY OF LIABILITY.
 *
 */

#include <linux/device.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/uio_driver.h>
#include <linux/delay.h>

#define DEVICE_NAME "jaguar2"

#define PCIE_VENDOR_ID 0x101B
#define PCIE_DEVICE_ID 0xB003

struct uio_jaguar2 {
    struct uio_info uio;
    /* Private data */
    spinlock_t lock;
    unsigned long flags;
    struct pci_dev *pdev;
};

static irqreturn_t jaguar2_handler(int irq, struct uio_info *dev_info)
{
    struct uio_jaguar2 *priv = dev_info->priv;

    //printk("Jaguar2: Got IRQ, disabling\n");
        
    if (!test_and_set_bit(0, &priv->flags))
        disable_irq_nosync(irq);

    return IRQ_HANDLED;
}

static int jaguar2_irqcontrol(struct uio_info *dev_info, s32 irq_on)
{
    struct uio_jaguar2 *priv = dev_info->priv;
    unsigned long flags;

    spin_lock_irqsave(&priv->lock, flags);
    if (irq_on) {
        if (test_and_clear_bit(0, &priv->flags)) {
            //printk("Jaguar2: Enable IRQ\n");
            enable_irq(dev_info->irq);
        }
    } else {
        if (!test_and_set_bit(0, &priv->flags)) {
            //printk("Jaguar2: Disable IRQ\n");
            disable_irq(dev_info->irq);
        }
    }
    spin_unlock_irqrestore(&priv->lock, flags);

    return 0;
}

static int jaguar2_pci_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
    struct uio_jaguar2 *priv;
    struct uio_info *info;
    void __iomem *chipid_reg;

    priv = kzalloc(sizeof(struct uio_jaguar2), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;
    info = &priv->uio;
    info->priv = priv;

    if (pci_enable_device(dev))
        goto out_free;

    if (pci_request_regions(dev, DEVICE_NAME))
        goto out_disable;

    /* BAR0 = registers, BAR1 = CONFIG, BAR2 = DDR (unused) */
    info->mem[0].addr = pci_resource_start(dev, 0);
    if (!info->mem[0].addr)
        goto out_release;
    info->mem[0].size = pci_resource_len(dev, 0);
    info->mem[0].memtype = UIO_MEM_PHYS;
    info->mem[0].internal_addr = ioremap(info->mem[0].addr, info->mem[0].size);

#if 1
    info->mem[1].addr = pci_resource_start(dev, 1);
    if (!info->mem[1].addr)
        goto out_release;
    info->mem[1].size = pci_resource_len(dev, 1);
    info->mem[1].memtype = UIO_MEM_PHYS;
#endif

    info->name = "Jaguar2 RefBoard";
    info->version = "1.0.0";
    info->irq = dev->irq;
    info->handler = jaguar2_handler;
    info->irqcontrol = jaguar2_irqcontrol;

    spin_lock_init(&priv->lock);
    priv->flags = 0; /* interrupt is enabled to begin with */
    priv->pdev = dev;

    if (uio_register_device(&dev->dev, info))
        goto out_unmap;

    pci_set_drvdata(dev, info);
    chipid_reg = info->mem[0].internal_addr + (0x01010000);
    dev_info(&dev->dev, "Found %s, UIO device - IRQ %ld, id 0x%08x.\n", info->name, info->irq, ioread32(chipid_reg));

    return 0;

out_unmap:
    iounmap(info->mem[0].internal_addr);
out_release:
    pci_release_regions(dev);
out_disable:
    pci_disable_device(dev);
out_free:
    kfree(info);
    return -ENODEV;
}


static void jaguar2_pci_remove(struct pci_dev *dev)
{
    struct uio_info *info = pci_get_drvdata(dev);

    uio_unregister_device(info);
    iounmap(info->mem[0].internal_addr);
    pci_release_regions(dev);
    pci_disable_device(dev);
    pci_set_drvdata(dev, NULL);

    kfree(info->priv);
}

static pci_ers_result_t jaguar2_error_detected(struct pci_dev *dev,
                                               enum pci_channel_state state)
{
    printk(KERN_ERR DEVICE_NAME ": PCI error\n");
    if (state == pci_channel_io_perm_failure)
        return PCI_ERS_RESULT_DISCONNECT;
    if (state == pci_channel_io_frozen)
        return PCI_ERS_RESULT_NEED_RESET;
    return PCI_ERS_RESULT_CAN_RECOVER;
}

static pci_ers_result_t jaguar2_link_reset(struct pci_dev *dev)
{
    printk(KERN_INFO DEVICE_NAME ": link reset\n");
    return 0;
}

static pci_ers_result_t jaguar2_slot_reset(struct pci_dev *dev)
{
    printk(KERN_INFO DEVICE_NAME ": slot reset\n");
    return 0;
}

static void jaguar2_resume(struct pci_dev *dev)
{
    printk(KERN_INFO DEVICE_NAME ": resume\n");
}

static struct pci_error_handlers jaguar2_errors = {
    .error_detected = jaguar2_error_detected,
    .link_reset = jaguar2_link_reset,
    .slot_reset = jaguar2_slot_reset,
    .resume = jaguar2_resume,
};


static struct pci_device_id jaguar2_pci_ids[] = {
    {
        .vendor =	PCIE_VENDOR_ID,
        .device =	PCIE_DEVICE_ID,
        .subvendor =	PCI_ANY_ID,
        .subdevice =	PCI_ANY_ID,
    },
    { 0, }
};

static struct pci_driver jaguar2_pci_driver = {
    .name = DEVICE_NAME,
    .id_table = jaguar2_pci_ids,
    .probe = jaguar2_pci_probe,
    .remove = jaguar2_pci_remove,
    .err_handler = &jaguar2_errors,
};

static int __init jaguar2_init_module(void)
{
    return pci_register_driver(&jaguar2_pci_driver);
}

static void __exit jaguar2_exit_module(void)
{
    pci_unregister_driver(&jaguar2_pci_driver);
}

module_init(jaguar2_init_module);
module_exit(jaguar2_exit_module);

MODULE_DEVICE_TABLE(pci, jaguar2_pci_ids);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Lars Povlsen <lpovlsen@vitesse.com>");
