// Copyright (c) 2016 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/lib/bluetooth_chooser.h"

#include "shell/common/gin_converters/callback_converter.h"
#include "shell/common/gin_helper/dictionary.h"

namespace gin {

template <>
struct Converter<electron::BluetoothChooser::DeviceInfo> {
  static v8::Local<v8::Value> ToV8(
      v8::Isolate* isolate,
      const electron::BluetoothChooser::DeviceInfo& val) {
    gin_helper::Dictionary dict = gin::Dictionary::CreateEmpty(isolate);
    dict.Set("deviceName", val.device_name);
    dict.Set("deviceId", val.device_id);
    return gin::ConvertToV8(isolate, dict);
  }
};

}  // namespace gin

namespace electron {

namespace {

const int kMaxScanRetries = 5;

void OnDeviceChosen(const content::BluetoothChooser::EventHandler& handler,
                    const std::string& device_id) {
  LOG(INFO) << "Bluetooth device choosen" << device_id;
  if (device_id.empty()) {
    handler.Run(content::BluetoothChooserEvent::CANCELLED, device_id);
  } else {
    handler.Run(content::BluetoothChooserEvent::SELECTED, device_id);
  }
}

}  // namespace

BluetoothChooser::BluetoothChooser(api::WebContents* contents,
                                   const EventHandler& event_handler)
    : api_web_contents_(contents), event_handler_(event_handler) {}

BluetoothChooser::~BluetoothChooser() {
  event_handler_.Reset();
}

void BluetoothChooser::SetAdapterPresence(AdapterPresence presence) {
  switch (presence) {
    case AdapterPresence::ABSENT:
      LOG(INFO) << "BluetoothChooser::SetAdapterPresence, presence is ABSENT";
      break;
    case AdapterPresence::POWERED_OFF:
      // Chrome currently directs the user to system preferences
      // to grant bluetooth permission for this case, should we
      // do something similar ?
      // https://chromium-review.googlesource.com/c/chromium/src/+/2617129
      LOG(INFO)
          << "BluetoothChooser::SetAdapterPresence, presence is POWERED_OFF";
      break;
    case AdapterPresence::UNAUTHORIZED:
      LOG(INFO)
          << "BluetoothChooser::SetAdapterPresence, presence is UNAUTHORIZED";
      event_handler_.Run(content::BluetoothChooserEvent::CANCELLED, "");
      break;
    case AdapterPresence::POWERED_ON:
      LOG(INFO)
          << "BluetoothChooser::SetAdapterPresence, presence is POWERED_ON";
      break;
  }
}

void BluetoothChooser::ShowDiscoveryState(DiscoveryState state) {
  switch (state) {
    case DiscoveryState::FAILED_TO_START:
      LOG(INFO) << "BluetoothChooser::ShowDiscoveryState, discovery state is "
                   "FAILED_TO_START";
      refreshing_ = false;
      event_handler_.Run(content::BluetoothChooserEvent::CANCELLED, "");
      break;
    case DiscoveryState::IDLE:
      LOG(INFO)
          << "BluetoothChooser::ShowDiscoveryState, discovery state is IDLE";
      refreshing_ = false;
      if (device_map_.empty()) {
        LOG(INFO) << "BluetoothChooser::ShowDiscoveryState, discovery state is "
                     "IDLE, device map is empty";
        auto event = ++num_retries_ > kMaxScanRetries
                         ? content::BluetoothChooserEvent::CANCELLED
                         : content::BluetoothChooserEvent::RESCAN;
        event_handler_.Run(event, "");
      } else {
        LOG(INFO)
            << "BluetoothChooser::ShowDiscoveryState, discovery state is IDLE, "
               "device map has items about to call select-bluetooth-device";
        bool prevent_default = api_web_contents_->Emit(
            "select-bluetooth-device", GetDeviceList(),
            base::BindOnce(&OnDeviceChosen, event_handler_));
        if (!prevent_default) {
          auto it = device_map_.begin();
          auto device_id = it->first;
          event_handler_.Run(content::BluetoothChooserEvent::SELECTED,
                             device_id);
        }
      }
      break;
    case DiscoveryState::DISCOVERING:
      LOG(INFO) << "BluetoothChooser::ShowDiscoveryState, discovery state is "
                   "DISCOVERING";
      // The first time this state fires is due to a rescan triggering so set a
      // flag to ignore devices
      if (!refreshing_) {
        LOG(INFO) << "BluetoothChooser::ShowDiscoveryState, discovery state is "
                     "DISCOVERING, not refreshing; set refreshing to true";
        refreshing_ = true;
      } else {
        // The second time this state fires we are now safe to pick a device
        LOG(INFO) << "BluetoothChooser::ShowDiscoveryState, discovery state is "
                     "DISCOVERING, refreshing; set refreshing to false";
        refreshing_ = false;
      }
      break;
  }
}

void BluetoothChooser::AddOrUpdateDevice(const std::string& device_id,
                                         bool should_update_name,
                                         const std::u16string& device_name,
                                         bool is_gatt_connected,
                                         bool is_paired,
                                         int signal_strength_level) {
  LOG(INFO) << "BluetoothChooser::AddOrUpdateDevice: " >> device_id >>
      ", named: " >> device_name;
  if (refreshing_) {
    LOG(INFO) << "BluetoothChooser::AddOrUpdateDevice refreshing so ignore " >>
        device_id >> ", named: " >> device_name;
    // If the list of bluetooth devices is currently being generated don't fire
    // an event
    return;
  }
  bool changed = false;
  auto entry = device_map_.find(device_id);
  if (entry == device_map_.end()) {
    device_map_[device_id] = device_name;
    changed = true;
  } else if (should_update_name) {
    entry->second = device_name;
    changed = true;
  }

  if (changed) {
    // Emit a select-bluetooth-device handler to allow for user to listen for
    // bluetooth device found.
    bool prevent_default = api_web_contents_->Emit(
        "select-bluetooth-device", GetDeviceList(),
        base::BindOnce(&OnDeviceChosen, event_handler_));

    // If emit not implimented select first device that matches the filters
    //  provided.
    if (!prevent_default) {
      event_handler_.Run(content::BluetoothChooserEvent::SELECTED, device_id);
    }
  }
}

std::vector<electron::BluetoothChooser::DeviceInfo>
BluetoothChooser::GetDeviceList() {
  std::vector<electron::BluetoothChooser::DeviceInfo> vec;
  vec.reserve(device_map_.size());
  for (const auto& it : device_map_) {
    DeviceInfo info = {it.first, it.second};
    vec.push_back(info);
  }

  return vec;
}

}  // namespace electron
