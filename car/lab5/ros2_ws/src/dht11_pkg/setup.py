from setuptools import find_packages, setup

package_name = 'dht11_pkg'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='a720a',
    maintainer_email='a720a@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'monitor = dht11_pkg.dht11_monitor:main',
            'publisher = dht11_pkg.dht11_publisher:main',
            'subscriber = dht11_pkg.dht11_subscriber:main',
        ],
    },
)
