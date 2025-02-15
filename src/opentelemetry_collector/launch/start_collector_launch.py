from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration, TextSubstitution
from launch_ros.substitutions import ExecutableInPackage
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

import os


def generate_launch_description():
    return LaunchDescription(
        [
            DeclareLaunchArgument(
                name="config",
                description="Locations to the config file",
                default_value=os.path.join(
                    get_package_share_directory("opentelemetry_collector"),
                    "config",
                    "config.yaml",
                ),
            ),
            DeclareLaunchArgument(
                name="feature-gates",
                description="Comma-delimited list of feature gate identifiers.",
                default_value="",
            ),
            ExecuteProcess(
                cmd=[
                    ExecutableInPackage(
                        package="opentelemetry_collector", executable="otelcol-contrib"
                    ),
                    "--config",
                    LaunchConfiguration("config"),
                    "--feature-gates",
                    LaunchConfiguration("feature-gates"),
                ]
            ),
        ]
    )
