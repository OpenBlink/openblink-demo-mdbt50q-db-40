# OpenBlink Demo MDBT50Q-DB-40

[![English](https://img.shields.io/badge/language-English-blue.svg)](README.md)
[![中文](https://img.shields.io/badge/language-中文-red.svg)](README.zh-CN.md)
[![日本語](https://img.shields.io/badge/language-日本語-green.svg)](README.ja.md)
[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/OpenBlink/openblink-demo-mdbt50q-db-40)

## OpenBlink 是什么

**_OpenBlink_** 是一个从 **_ViXion Blink_** 分叉的开源项目。

- 可以使用高生产力的轻量级语言 Ruby 来开发嵌入式设备。
- 程序重写和调试控制台完全无线。（蓝牙低功耗）
- 重写时间少于 0.1 秒，不需要微处理器重启。（我们称之为"Blink"）

## 如何获取 OpenBlink

要克隆仓库并初始化子模块，请运行以下命令：

```console
$ git clone https://github.com/OpenBlink/openblink-demo-mdbt50q-db-40.git
$ git submodule init
$ git submodule update
```

## 已验证的硬件

以下硬件平台已通过 OpenBlink 测试：

- Raytac MDBT50Q-DB-40 (Board target: raytac_mdbt50q_db_40/nrf52840)

## 开发环境版本

- nRF Connect SDK toolchain v2.9.0
- nRF Connect SDK v2.9.0
