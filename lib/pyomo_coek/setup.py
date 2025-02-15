from setuptools import setup, find_packages
from distutils.core import Extension


setup(
    name="pyomo_coek",
    version="0.1.0.dev0",
    packages=find_packages(),
    ext_modules=[],
    description="Poek-based pyomo components",
    author="pyomo_coek developers",
    maintainer_email="wehart@sandia.gov",
    license="Revised BSD",
    install_requires=["pyomo>=6.4.1", "poek", "munch"],
    include_package_data=True,
    scripts=[],
    python_requires=">=3.7",
    classifiers=[
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "License :: OSI Approved :: BSD License",
        "Operating System :: OS Independent",
    ],
)
