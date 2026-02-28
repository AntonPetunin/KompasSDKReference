# KompasSDK Documentation Source

This repository serves as the **primary source** and **local data provider** for the [alekseyvm/kompas-sdk-help](https://github.com/alekseyvm/kompas-sdk-help) documentation site.

## Purpose

This project contains the raw, source documentation for the **KompasSDK** library and its integration tools. It is designed to be the "single source of truth" from which the user-friendly, published documentation at `kompas-sdk-help` is generated and updated.

## Development Context

This repository is maintained for the **confidential development of a commercial proposal** targeted at:

- **ASCON (Аскон)** – the developer of KOMPAS-3D
- **Two primary stakeholders:**
  - **Vladimir Lipin (Владимир Липин)** – Head of Technical Support at ASCON
  - **Anton Vasilyevich Tsivarev (Антон Васильевич Циварев)** – Product Manager

The documentation and SDK development are being prepared as part of a strategic commercial offering to demonstrate integration capabilities and value proposition for KOMPAS-3D ecosystem enhancement.

### Relationship with `kompas-sdk-help`
*   **This Repository (Parent/Source):** Holds the original documentation files, configuration, and assets in an editable format. Development occurs here privately before proposal presentation.
*   **[alekseyvm/kompas-sdk-help](https://github.com/alekseyvm/kompas-sdk-help) (Target/Output):** Contains the built, static website generated from this source. That repository is configured to pull updates from here to keep the public documentation in sync.

## What's Documented Here

The source covers all aspects of the KompasSDK ecosystem, including:

*   **Core Library API:** Detailed documentation for library components and server logic.
*   **KOMPAS-3D Integration:** Guides on library registration, connecting applications, and using in-software commands.
*   **Serialization Workflows:** Export/import procedures, mass properties checks, and version management.

## Structure for Documentation Source

The source is organized to be compatible with documentation generators (like Diplodoc used in the target repository):
