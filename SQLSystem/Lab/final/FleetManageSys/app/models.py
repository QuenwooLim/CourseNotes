from datetime import date
from decimal import Decimal
from typing import Optional, Dict, Any

class Fleet:
    def __init__(self, fleet_id: int, fleet_name: str, center_id: int):
        self.fleet_id = fleet_id
        self.fleet_name = fleet_name
        self.center_id = center_id

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Fleet':
        return cls(
            fleet_id=data.get('fleet_id'),
            fleet_name=data.get('fleet_name'),
            center_id=data.get('center_id')
        )

class DistributionCenter:
    def __init__(self, center_id: int, center_name: str, address: str):
        self.center_id = center_id
        self.center_name = center_name
        self.address = address

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'DistributionCenter':
        return cls(
            center_id=data.get('center_id'),
            center_name=data.get('center_name'),
            address=data.get('address')
        )

class Truck:
    def __init__(self, plate_number: str, max_load: float, max_volume: float, current_status: str, fleet_id: int):
        self.plate_number = plate_number
        self.max_load = max_load
        self.max_volume = max_volume
        self.current_status = current_status
        self.fleet_id = fleet_id

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Truck':
        return cls(
            plate_number=data.get('plate_number'),
            max_load=float(data.get('max_load') or 0),
            max_volume=float(data.get('max_volume') or 0),
            current_status=data.get('current_status'),
            fleet_id=data.get('fleet_id')
        )

class Driver:
    def __init__(self, driver_id: str, name: str, license_level: str, phone: str, hire_date: date, fleet_id: int):
        self.driver_id = driver_id
        self.name = name
        self.license_level = license_level
        self.phone = phone
        self.hire_date = hire_date
        self.fleet_id = fleet_id

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Driver':
        return cls(
            driver_id=data.get('driver_id'),
            name=data.get('name'),
            license_level=data.get('license_level'),
            phone=data.get('phone'),
            hire_date=data.get('hire_date'),
            fleet_id=data.get('fleet_id')
        )

class Order:
    def __init__(self, order_id: str, weight: float, volume: float, destination: str, 
                 status: str, create_time, truck_plate: str):
        self.order_id = order_id
        self.weight = weight
        self.volume = volume
        self.destination = destination
        self.status = status
        self.create_time = create_time
        self.truck_plate = truck_plate

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Order':
        return cls(
            order_id=data.get('order_id'),
            weight=float(data.get('weight') or 0),
            volume=float(data.get('volume') or 0),
            destination=data.get('destination'),
            status=data.get('status'),
            create_time=data.get('create_time'),
            truck_plate=data.get('truck_plate')
        )

class ExceptionRecord:
    def __init__(self, record_id: int, exception_type: str, occur_time, 
                 fine_amount: float, handle_status: str, truck_plate: str, driver_id: str):
        self.record_id = record_id
        self.exception_type = exception_type
        self.occur_time = occur_time
        self.fine_amount = fine_amount
        self.handle_status = handle_status
        self.truck_plate = truck_plate
        self.driver_id = driver_id

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'ExceptionRecord':
        return cls(
            record_id=data.get('record_id'),
            exception_type=data.get('exception_type'),
            occur_time=data.get('occur_time'),
            fine_amount=float(data.get('fine_amount') or 0),
            handle_status=data.get('handle_status'),
            truck_plate=data.get('truck_plate'),
            driver_id=data.get('driver_id')
        )

class Supervisor:
    def __init__(self, supervisor_id: str, name: str, phone: str, fleet_id: int):
        self.supervisor_id = supervisor_id
        self.name = name
        self.phone = phone
        self.fleet_id = fleet_id
        # Password 不建议要在列表中展示，故不放在 init 中或者不读取

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Supervisor':
        return cls(
            supervisor_id=data.get('supervisor_id'),
            name=data.get('name'),
            phone=data.get('phone'),
            fleet_id=data.get('fleet_id')
        )

class HistoryLog:
    def __init__(self, log_id: int, target_id: str, old_value: str, new_value: str, change_time, operation_type: str):
        self.log_id = log_id
        self.target_id = target_id
        self.old_value = old_value
        self.new_value = new_value
        self.change_time = change_time
        self.operation_type = operation_type

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'HistoryLog':
        return cls(
            log_id=data.get('log_id'),
            target_id=data.get('target_id'),
            old_value=data.get('old_value'),
            new_value=data.get('new_value'),
            change_time=data.get('change_time'),
            operation_type=data.get('operation_type')
        )

class PathCostRule:
    def __init__(self, rule_id: int, center_id: int, target_province: str, base_price_per_km_ton: float, traffic_factor: float):
        self.rule_id = rule_id
        self.center_id = center_id
        self.target_province = target_province
        self.base_price_per_km_ton = base_price_per_km_ton
        self.traffic_factor = traffic_factor

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'PathCostRule':
        return cls(
            rule_id=data.get('rule_id'),
            center_id=data.get('center_id'),
            target_province=data.get('target_province'),
            base_price_per_km_ton=float(data.get('base_price_per_km_ton') or 0),
            traffic_factor=float(data.get('traffic_factor') or 1.0)
        )

class FleetEfficiencyScore:
    def __init__(self, fleet_id: int, avg_delivery_hours: float, safety_score: int, cost_efficiency_index: float):
        self.fleet_id = fleet_id
        self.avg_delivery_hours = avg_delivery_hours
        self.safety_score = safety_score
        self.cost_efficiency_index = cost_efficiency_index

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'FleetEfficiencyScore':
        return cls(
            fleet_id=data.get('fleet_id'),
            avg_delivery_hours=float(data.get('avg_delivery_hours') or 0),
            safety_score=data.get('safety_score'),
            cost_efficiency_index=float(data.get('cost_efficiency_index') or 0)
        )
